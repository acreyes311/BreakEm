#!/bin/bash

# 'chmod +x compile.sh' for permission 
clang++ -o main -std=c++11 -lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio ./main.cpp ./Ball.cpp ./Platform.cpp ./Brick.cpp && ./main