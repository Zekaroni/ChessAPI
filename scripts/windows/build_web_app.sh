#!/bin/bash

# Ensure the build directory exists
mkdir -p build

# Compile C++ source files for the web using em++
em++ main.cpp chess/ChessLogic.cpp video/ChessGUI.cpp -o build/index.html \
  -I/c/Users/User2/Desktop/ChessAPI-webdeps/raylib-6.0/src \
  /c/Users/User2/Desktop/ChessAPI-webdeps/raylib-6.0/src/libraylib.web.a \
  -s USE_GLFW=3 \
  -s WASM=1 \
  -s DEFAULT_TO_CXX=1 \
  -s ASYNCIFY=1 \
  -O1 \
  --shell-file /c/Users/User2/Desktop/ChessAPI-webdeps/raylib-6.0/src/shell.html \
  -std=c++17