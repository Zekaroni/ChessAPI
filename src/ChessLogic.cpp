#include <iostream>
#include <array>
#include <cmath>

#include "../include/ChessLogic.h"

ChessLogic::ChessLogic()
{
    //White Starting Positions
    pieceBitboards[WHITE].pawnBitboard   = 0b11111111 << 8;
    pieceBitboards[WHITE].bishopBitboard = 0b00100100;
    pieceBitboards[WHITE].knightBitboard = 0b01000010;
    pieceBitboards[WHITE].rookBitboard   = 0b10000001;
    pieceBitboards[WHITE].queenBitboard  = 0b00010000;
    pieceBitboards[WHITE].kingBitboard   = 0b00001000;
    //Black Starting Positions
    pieceBitboards[BLACK].pawnBitboard   = (uint64_t)0b11111111 << 48;
    pieceBitboards[BLACK].bishopBitboard = (uint64_t)0b00100100 << 56;
    pieceBitboards[BLACK].knightBitboard = (uint64_t)0b01000010 << 56;
    pieceBitboards[BLACK].rookBitboard   = (uint64_t)0b10000001 << 56;
    pieceBitboards[BLACK].queenBitboard  = (uint64_t)0b00010000 << 56;
    pieceBitboards[BLACK].kingBitboard   = (uint64_t)0b00001000 << 56;
}

void ChessLogic::printBoard()
{
    occupiedBoard = pieceBitboards[WHITE].bishopBitboard | pieceBitboards[WHITE].knightBitboard
                  | pieceBitboards[WHITE].pawnBitboard   | pieceBitboards[WHITE].rookBitboard
                  | pieceBitboards[WHITE].queenBitboard  | pieceBitboards[WHITE].kingBitboard
                  | pieceBitboards[BLACK].bishopBitboard | pieceBitboards[BLACK].knightBitboard
                  | pieceBitboards[BLACK].pawnBitboard   | pieceBitboards[BLACK].rookBitboard
                  | pieceBitboards[BLACK].queenBitboard  | pieceBitboards[BLACK].kingBitboard;

    print_bits(knightMoveTable[3]);
}
void ChessLogic::print_bits(uint64_t value)
{
    for (int i = 63; i >= 0; i--)
    {
        std::cout << ((value >> i) & 1);
        if ((i) % 8 == 0)
        {
            std::cout << '\n';
        }
    }
}
void ChessLogic::calculateKnightMoves()
{
    for (int i = 63; i >= 0; i--)
    {
        int y = int(i/8);
        int x = int(i%8);
        uint64_t currentBitBoard = 0;
        std::array<Point,8> point = {{{-2,1},{-2,-1},{2,1},{2,-1},{1,-2},{-1,-2},{1,2},{-1,2}}};
       
        for (int j=0;j<8;j++)
        {
            if (!(x+point[j].x < 0 || x+point[j].x > 7 || y+point[j].y < 0 || y+point[j].y > 7))
            {
                currentBitBoard |= (1ull<<(8*(y+point[j].y))+x+point[j].x);
            }
        }

        knightMoveTable[i] = currentBitBoard;
    }
}