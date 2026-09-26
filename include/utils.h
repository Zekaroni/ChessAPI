#pragma once
#include <raylib.h>
#include <cstdint>
#include <iostream>
#include <array>
#include <cmath>


using bitboard_t = std::uint64_t;

#define WHITESIDE 0
#define BLACKSIDE 1

namespace CHESS_GLOBALS
{
    namespace COLORS
    {
        Color HIGHLIGHT  = {255,0,0,128};
        Color BACKGROUND = {100,120,100,255};
    }

    uint8_t PIECE_VALUE[7] =
    {
        0,1,3,3,5,9,100
    };

    enum PIECES
    {
        EMPTY,
        PAWN,
        KNIGHT,
        BISHOP,
        ROOK,
        QUEEN,
        KING
    };
    
    namespace CONTROLS
    {
        int CYCLE_BITBOARD_UP    = KEY_UP;
        int CYCLE_BITBOARD_DOWN  = KEY_DOWN;
        int CYCLE_BITBOARD_RIGHT = KEY_RIGHT;
        int CYCLE_BITBOARD_LEFT  = KEY_LEFT;
    }
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

