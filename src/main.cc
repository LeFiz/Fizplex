#include <chrono>
#include <iostream>
#include <random>

#include "base.h"
#include "colmatrix.h"
#include "lp.h"
#include "svector.h"

#define EXPECT(statement)                                                      \
  (statement ? std::cerr : (std::cerr << "Error! Expected true got false! In " \
                                      << __FUNCTION__ << " (Line " << __LINE__ \
                                      << " in " << __FILE__ << ")\n"))

// 3rd pass-through macro is required as otherwise __LINE__
// would be taken literally instead of being expanded
// to a number
#define _LINE_NAME(name, line) name##line
#define LINE_NAME(name, line) _LINE_NAME(name, line)
#define UNIQUE_NAME(_class, _method) LINE_NAME(_class##_method, __LINE__)
#define REG_NAME(name) name##reg

#define Test(_class, _method, _desc)                                           \
  _Test(UNIQUE_NAME(_class, _method), #_class, #_method, _desc)

#define _Test(name, _class, _method, _desc)                                    \
  void name();                                                                 \
  Registration REG_NAME(name)(name, _class, _method, _desc);                   \
  void name()

void test_base_no_reordering();
void test_base_with_reordering();
void test_base_singular_matrix();

using TestFunction = void (*)();
using Timer = std::chrono::high_resolution_clock;

class TestRunner {
public:
  struct Test {
    TestFunction f;
    std::string clss;
    std::string method;
    std::string desc;
  };

  static void reg(TestFunction f, std::string c, std::string m, std::string d) {
    tests.push_back({f, c, m, d});
  }
  static void run() {
    for (auto &t : tests) {
      std::cout << t.clss << "." << t.method << " " << t.desc << std::endl;
      auto start = Timer::now();
      t.f();
      auto end = Timer::now();
      auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(
          end - start);
      std::cout << duration.count() * 1000 << "ms" << std::endl;
    }
  }

private:
  static std::vector<Test> tests;
};

std::vector<TestRunner::Test> TestRunner::tests;

class Registration {
public:
  Registration(TestFunction f, std::string _class, std::string _method,
               std::string _desc) {
    TestRunner::reg(f, _class, _method, _desc);
  }
};

int main() {
  TestRunner::run();
  return 0;
}

Test(SVector, all, "works") {
  SVector v;
  EXPECT(v.length() == 0);
  SVector vv = {{3, 0.000f}, {19, 0}};
  EXPECT(v == vv);
  v.add_value(3, -0.12345);
  EXPECT(v.length() == 1);
  EXPECT(v != vv);
  EXPECT(vv != v);

  SVector w = {{0, -2.0}, {1, 3}, {2, 9}};
  SVector ww = {{0, -2}, {1, 3}, {2, 9}, {7, 0}};
  EXPECT(w.length() == 3);
  EXPECT(ww.length() == 4);
  EXPECT(w == ww);
  EXPECT(ww == w);
}

Test(ColMatrix, all, "works") {
  ColMatrix<3> m = {
      {{0, 7}, {2, -3}}, {{0, 2}, {1, 3}, {2, 4}}, {{0, 1}, {1, -1}, {2, -2}}};
  ColMatrix<3> mm = {{{0, 7}, {2, -3}, {1, 0.0}},
                     {{0, 2}, {1, 3}, {2, 4}},
                     {{0, 1}, {1, -1}, {2, -2}}};
  EXPECT(m == mm);
  EXPECT(mm == m);
  SVector v = {{0, 2}, {1, 3}, {2, 4}};
  EXPECT(m.column(1) == v);
}

Test(ColMatrix, invert, "invertible, no re-ordering") {
  ColMatrix<3> m = {
      {{0, 7}, {2, -3}}, {{0, 2}, {1, 3}, {2, 4}}, {{0, 1}, {1, -1}, {2, -2}}};
  Base<3> b(m);
  SVector e0 = {{0, 1}};
  SVector e1 = {{1, 1}};
  SVector e2 = {{2, 1}};
  const SVector r0 = {{0, -2}, {1, 3}, {2, 9}};
  const SVector r1 = {{0, 8}, {1, -11}, {2, -34}};
  const SVector r2 = {{0, -5}, {1, 7}, {2, 21}};
  EXPECT(b.invert());
  b.updateVec(e0);
  b.updateVec(e1);
  b.updateVec(e2);
  EXPECT(e0 == r0);
  EXPECT(e1 == r1);
  EXPECT(e2 == r2);
}

Test(ColMatrix, invert, "invertible, with re-ordering") {
  ColMatrix<3> m = {
      {{1, 7}, {2, -3}}, {{0, 2}, {1, 3}, {2, 4}}, {{0, 1}, {1, -1}, {2, -2}}};
  Base<3> b(m);
  SVector e0 = {{0, 1}};
  SVector e1 = {{1, 1}};
  SVector e2 = {{2, 1}};
  const SVector r0 = {{0, -2.0 / 71}, {1, 17.0 / 71}, {2, 37.0 / 71}};
  const SVector r1 = {{0, 8.0 / 71}, {1, 3.0 / 71}, {2, -6.0 / 71}};
  const SVector r2 = {{0, -5.0 / 71}, {1, 7.0 / 71}, {2, -14.0 / 71}};
  EXPECT(b.invert());
  b.updateVec(e0);
  b.updateVec(e1);
  b.updateVec(e2);
  EXPECT(e0 == r0);
  EXPECT(e1 == r1);
  EXPECT(e2 == r2);
}

Test(ColMatrix, invert, "singular") {
  Base<3> b;
  EXPECT(b.invert() == false);

  ColMatrix<3> m = {{{0, 1}, {1, 1}, {2, 1}},
                    {{0, 1}, {1, 1}, {2, 1}},
                    {{0, 1}, {1, 1}, {2, 1}}};
  b.setBase(m);
  EXPECT(b.invert() == false);
}
