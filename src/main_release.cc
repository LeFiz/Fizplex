#include <iostream>

#include "lp.h"
#include "mpsreader.h"
#include "simplex.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " file.mps\n";
    return 0;
  }
  const std::string filename = argv[1];
  fizplex::LP lp;
  try {
    lp = fizplex::MPSReader::read_lp(filename);
  } catch (const std::exception &e) {
    std::cerr << "Could not read file " << filename << "\n";
    std::cerr << "Reason: " << e.what() << "\n";
    return 1;
  }
  fizplex::Simplex spx(lp, 1);
  spx.solve();
  return 0;
}
