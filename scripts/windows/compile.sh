# Run from the project root
# g++ main.cpp -o ./main.exe \
#     -Os \
#     -DNDEBUG \
#     -flto \
#     -fno-exceptions \
#     -fno-rtti \
#     -fno-unwind-tables \
#     -fno-asynchronous-unwind-tables \
#     -ffunction-sections \
#     -fdata-sections \
#     -Wl,--gc-sections \
#     -Wl,--strip-all \
#     $(pkg-config --cflags --libs raylib)

g++ main.cpp -o ./bin/main.exe \
    -O2 \
    $(pkg-config --cflags --libs raylib)

