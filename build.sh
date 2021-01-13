#!/bin/bash

flagovi="-Wall -Werror -Wextra -Wno-unused-variable -Wno-sign-compare -include ../common.h -I../imgui"
no_cpp="-fno-rtti -fomit-frame-pointer -fno-exceptions -pipe -fstrict-return -fcolor-diagnostics -fdiagnostics-absolute-paths"

pushd object
pat='../imgui'
clang -c $no_cpp $pat/imgui_draw.cpp $pat/imgui_widgets.cpp $pat/imgui_tables.cpp $pat/imgui.cpp
clang -c $no_cpp $flagovi ../imgui_impl_sdl.cpp ../imgui_impl_opengl3.cpp
clang -c $no_cpp $flagovi ../main.cpp
clang -c $no_cpp $flagovi ../parser.cpp
popd

clang++ object/* -o program -lm -lSDL2 -lGLEW -lGL
