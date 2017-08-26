#ifndef TEST_H
#define TEST_H

#include <chrono>
#include <iostream>

#define EXPECT(statement)                                                      \
  (statement ? std::cerr                                                       \
             : (std::cerr << "\033[1;31mError! Expected true got false! In "   \
                          << __FUNCTION__ << " (Line " << __LINE__ << " in "   \
                          << __FILE__ << ")\033[0m\n"))

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
    tests().push_back({f, c, m, d});
  }
  static void run() {
    for (auto &t : tests()) {
      auto start = Timer::now();
      t.f();
      auto end = Timer::now();
      auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(
          end - start);
      std::cout << t.clss << "." << t.method << " " << t.desc;
      std::cout << "(" << duration.count() * 1000 << "ms"
                << ")" << std::endl;
    }
  }

private:
  static std::vector<Test> &tests() {
    static std::vector<Test> *_t = new std::vector<Test>(); // TODO leaking!
    return *_t;
  }
};

class Registration {
public:
  Registration(TestFunction f, std::string _class, std::string _method,
               std::string _desc) {
    TestRunner::reg(f, _class, _method, _desc);
  }
};

#endif
