#!/bin/bash

flagovi="-Wall -Werror -Wextra -Wno-unused-variable -Wno-sign-compare"
no_cpp="-fno-rtti -fomit-frame-pointer -fno-exceptions -pipe -fstrict-return -fcolor-diagnostics -fdiagnostics-absolute-paths"

pushd object
clang++ -c $no_cpp $flagovi ../ft9k.hpp
clang++ -c -std=c++17  $flagovi ../ft9k.cpp
clang++ -c  $flagovi ../example.cpp
popd

clang++ object/* -o example -lm -lSDL2 -lGLEW -lGL
