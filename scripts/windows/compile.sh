# Run from the project root
g++ main.cpp -o ./main.exe \
    $(pkg-config --cflags --libs raylib)

