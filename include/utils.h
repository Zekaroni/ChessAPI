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
    Color HIGHLIGHT = {255,0,0,128};
}


struct playerPiecesBitboard
{
    bitboard_t pawnBitboard;
    bitboard_t knightBitboard;
    bitboard_t bishopBitboard;
    bitboard_t rookBitboard;
    bitboard_t queenBitboard;
    bitboard_t kingBitboard;
};

struct Point
{
    int x;
    int y;
};

