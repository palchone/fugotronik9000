#!/bin/bash

clang -Wall -Werror -Wextra -Wno-unused-variable -Wno-sign-compare -include common.h parser.cpp -o program
