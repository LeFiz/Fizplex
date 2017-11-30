mkdir build
cd build
CXX=/usr/bin/g++-7 CC=/usr/bin/gcc-7 cmake ..
make -j4
cd ..
./build/tests
