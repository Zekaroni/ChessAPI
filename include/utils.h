#pragma once
#include <raylib.h>
#include <cstdint>
#include <iostream>
#include <array>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <vector>
#include <chrono>

using bitboard_t  = std::uint64_t;
using piece_t     = uint8_t;
using Clock = std::chrono::steady_clock;

#include "logging.h"

#define WHITESIDE    0
#define BLACKSIDE    1
#define NULLSIDE     2
#define PLAYER_COUNT 2
#define ROW_COUNT    8
#define COLUMN_COUNT 8
#define PIECE_TEXTURE_COUNT 12

Logging loggingHelper = Logging(); // GLOBAL used for logging

namespace CHESS_GLOBALS
{
    namespace COLORS
    {
        Color WHITE_SIDE  = {238, 220, 151, 255};
        Color BLACK_SIDE  = {150,  77,  34, 255};
        Color HIGHLIGHT   = {200,   0,   0, 100};
        Color BACKGROUND  = {100, 120, 100, 255};
        Color CURSOR      = {000, 200, 100, 100};

        Color* PLAYERS[2] = {&WHITE_SIDE, &BLACK_SIDE};
    }

    constexpr uint8_t PIECE_VALUE[7] =
    {
        0,1,3,3,5,9,100
    };

    const std::string STARTING_FEN_STRING = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    const char INDEX_TO_FEN_LETTER[8]     = {' ', 'p', 'n', 'b', 'r', 'q', 'k'};


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

    namespace FILES
    {
        constexpr const char* STRING = "abcdefgh";
    }
}


struct Point
{
    int x;
    int y;
};