rm main.exe
git pull
g++ main.cpp -o ./main.exe \
    $(pkg-config --cflags --libs raylib)

./main.exe