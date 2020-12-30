#!/bin/bash

clang -Wall -Werror -Wextra -Wno-unused-variable -Wno-sign-compare -include common.h parser.cpp note_set.cpp -o program
