#pragma once
#include <raylib.h>
#include <cstdint>
#include <iostream>
#include <array>
#include <cmath>

using bitboard_t = std::uint64_t;

#define WHITESIDE 0
#define BLACKSIDE 1

namespace CHESS_COLORS
{
    Color HIGHLIGHT = {0,0,0,0};
}

