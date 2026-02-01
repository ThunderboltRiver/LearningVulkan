rm -rf build/Xcode
mkdir -p build/Xcode
cmake -G Xcode -S . -B build/Xcode
cmake --build build/Xcode