#!/bin/bash
set -e

c++ -std=c++17 -O2 -o main \
  ./main.cpp ./Ball.cpp ./Platform.cpp ./Brick.cpp \
  $(pkg-config --cflags --libs sfml-graphics sfml-window sfml-system sfml-audio)

./main