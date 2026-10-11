g++ main.cpp ./chess/ChessLogic.cpp ./video/ChessGUI.cpp \
    -o ./bin/main.exe \
    -O2 \
    -DNDEBUG \
    -flto \
    -fno-exceptions \
    -fno-rtti \
    -fno-unwind-tables \
    -fno-asynchronous-unwind-tables \
    -ffunction-sections \
    -fdata-sections \
    -Wl,--gc-sections \
    -Wl,--strip-all \
    $(pkg-config --cflags --libs raylib)
