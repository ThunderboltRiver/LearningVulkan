rm -rf build
mkdir build
cd build
cmake -G Xcode ..
cmake --build . -v
cd ../