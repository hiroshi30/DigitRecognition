@echo off

cmake -S . -B build -G "MinGW Makefiles" -D CMAKE_BUILD_TYPE=Debug
cmake --build build
