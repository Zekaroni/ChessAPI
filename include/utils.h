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
#include <thread>

using bitboard_t  = std::uint64_t;
using piece_t     = uint8_t;
using Clock = std::chrono::steady_clock;

#define WHITESIDE    0
#define BLACKSIDE    1
#define NULLSIDE     2
#define PLAYER_COUNT 2
#define ROW_COUNT    8
#define COLUMN_COUNT 8
#define PIECE_TEXTURE_COUNT 12

namespace CHESS_GLOBALS
{
    namespace COLORS
    {
        inline Color WHITE_SIDE  = {238, 220, 151, 255};
        inline Color BLACK_SIDE  = {150,  77,  34, 255};
        inline Color HIGHLIGHT   = {200,   0,   0, 100};
        inline Color BACKGROUND  = {100, 120, 100, 255};
        inline Color CURSOR      = {000, 200, 100, 100};

        inline Color* PLAYERS[2] = {&WHITE_SIDE, &BLACK_SIDE};
    }

    inline constexpr uint8_t PIECE_VALUE[7] =
    {
        0,1,3,3,5,9,100
    };

    inline const std::string STARTING_FEN_STRING = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    inline const char INDEX_TO_FEN_LETTER[8]     = {' ', 'p', 'n', 'b', 'r', 'q', 'k'};
    inline const int TOTAL_CELLS = 64;

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
        inline constexpr int CYCLE_BITBOARD_UP    = KEY_UP;
        inline constexpr int CYCLE_BITBOARD_DOWN  = KEY_DOWN;
        inline constexpr int CYCLE_BITBOARD_RIGHT = KEY_RIGHT;
        inline constexpr int CYCLE_BITBOARD_LEFT  = KEY_LEFT;
    }

    namespace FILES
    {
        inline constexpr const char* STRING = "abcdefgh";
    }
}


struct Point
{
    int x;
    int y;
};


/// @brief Custom logging class for this project
class Logging
{
private:
    std::ofstream debugFile;
    bool _logStartupTime = false;
    bool _logMemoryUsage = false;
    bool _globalLog      = true;
    std::chrono::_V2::steady_clock::time_point _startupStartTime;
    std::chrono::_V2::steady_clock::time_point _startupEndTime;
    
    
public:
    Logging()
    {
        std::string debugFilePath = "./debug/output.txt";
        initDebugFile(debugFilePath);
    };
    void globalLog(bool log){_globalLog = log;};
    void logMemoryUsage(bool log){_logMemoryUsage = log;};

    void startStartupTimer(){_startupStartTime = Clock::now();};
    void endStartupTimer()  {_startupEndTime   = Clock::now();};
    
    bool logStartupTime(){return _logStartupTime;};
    void logStartupTime(bool log){_logStartupTime = log;};
    bool logStartupTimer(bool toFile = true)
    {
        if (toFile && !debugFile.is_open()) return false;
        std::ostream& output = toFile ? static_cast<std::ostream&>(debugFile) : std::cout;
        output << "Total startup time: " <<
                std::chrono::duration_cast<std::chrono::milliseconds>(_startupEndTime - _startupStartTime).count() <<
                "ms";
        output.flush();
        return output.good(); // true if good
        
    }


    bool debug_pieces(piece_t* boardState, bool toFile = true)
    {
        /*
            This function will print or write to the debug file
            the integer representation of each piece at every
            psotition formatted like the chess board.

            returns true if logs succesfully
        */
        if (toFile && !debugFile.is_open()) return false;
        std::ostream& output = toFile ? static_cast<std::ostream&>(debugFile) : std::cout;

        for (int i = 63; i >= 0; i--)
        {
            output << (int)boardState[i];
            if (i % 8 == 0) output << '\n';
        }

        output.flush();
        return output.good(); // true if good
    }

    bool debug_bitboard_array(bitboard_t* bitboardArray, int size, bool toFile = true)
    {
        /*
            This function outputs all the bitboards in an array
            in integer form to selected output.

            returns true if logs succesfully
        */
        if (toFile && !debugFile.is_open()) return 0;
        std::ostream& output = toFile ? static_cast<std::ostream&>(debugFile) : std::cout;

        for (int i = 0; i < size; i++)
        {
            output << bitboardArray[i] << '\n';
        }

        output.flush();
        return output.good();
    }

    bool debug_bitboard(bitboard_t value, bool toFile = true)
    {
        /*
            This funciton outputs the bitboard formatted like
            a chess board.

            returns true if logs succesfully
        */
        if (toFile && !debugFile.is_open()) return false;
        std::ostream& output = toFile ? static_cast<std::ostream&>(debugFile) : std::cout;

        for (int i = 63; i >= 0; i--)
        {
            output << ((value >> i) & 1);
            if (i % 8 == 0) output << '\n';
        }
        output << '\n';

        output.flush();
        return output.good();
    }

    bool initDebugFile(std::string filePath)
    {
        debugFile.open(filePath.c_str());
        if (!debugFile)
        {
            std::cerr << "Failed to open debug file.\n"; 
            return 0;
        }
        return 1;
    }

    bool outputToDebugFile(std::string output)
    {
        if (!debugFile.is_open()) return false;
        debugFile << output.c_str();
        return debugFile.good();
    }

    bool closeDebugFile()
    {
        if(!debugFile.is_open()) return 0;
        debugFile.close();
        return true;
    }


    bool streamToTerminal(std::string output)
    {
        /*
            This functions streams the output to one line.
            NOTE: This breaks if there are new lines
        */
        std::cout << output << '\r';
        return true;
    }

    bool printToTerminal(std::string output)
    {
        if (_globalLog)
        {
            std::cout << output.c_str();
            return true;
        }
        return false;
    }
};

extern Logging loggingHelper; // GLOBAL used for logging