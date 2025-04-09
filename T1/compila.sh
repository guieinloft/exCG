#!/bin/sh
g++ src/*.cpp src/Tools/*.cpp src/Effects/*.cpp src/*.c -lGL -lGLU -lglut -lm -std=c++11 -Wall -Wextra -O3
mv a.out ..
