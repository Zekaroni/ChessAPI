#pragma once
#include <raylib.h>
#include <cstdint>
#include <iostream>
#include <array>
#include <cmath>


using bitboard_t  = std::uint64_t;
using piece_t     = uint8_t;

#define WHITESIDE 0
#define BLACKSIDE 1
#define NULLSIDE  2

namespace CHESS_GLOBALS
{
    namespace COLORS
    {
        Color WHITE_SIDE  = {255, 255, 255, 255};
        Color BLACK_SIDE  = {  0,   0,   0, 255};
        Color HIGHLIGHT   = {255,   0,   0, 128};
        Color BACKGROUND  = {100, 120, 100, 255};
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

    enum PLAYER_PIECES
    {
        //White Pieces
        WHITE_PAWN   = 0b0000'0001,
        WHITE_KNIGHT = 0b0000'0010,
        WHITE_BISHOP = 0b0000'0011,
        WHITE_ROOK   = 0b0000'0100,
        WHITE_QUEEN  = 0b0000'0101,
        WHITE_KING   = 0b0000'0110,
        //Black Pieces
        BLACK_PAWN   = 0b0000'1001,
        BLACK_KNIGHT = 0b0000'1010,
        BLACK_BISHOP = 0b0000'1011,
        BLACK_ROOK   = 0b0000'1100,
        BLACK_QUEEN  = 0b0000'1101,
        BLACK_KING   = 0b0000'1110
    };
    
    namespace CONTROLS
    {
        constexpr int CYCLE_BITBOARD_UP    = KEY_UP;
        constexpr int CYCLE_BITBOARD_DOWN  = KEY_DOWN;
        constexpr int CYCLE_BITBOARD_RIGHT = KEY_RIGHT;
        constexpr int CYCLE_BITBOARD_LEFT  = KEY_LEFT;
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

