#include "ChessLogic.h"

ChessLogic::ChessLogic()
{
    //White Starting Positions
    pieceBitboards[WHITESIDE].pawnBitboard   = 0b11111111 << 8;
    pieceBitboards[WHITESIDE].bishopBitboard = 0b00100100;
    pieceBitboards[WHITESIDE].knightBitboard = 0b01000010;
    pieceBitboards[WHITESIDE].rookBitboard   = 0b10000001;
    pieceBitboards[WHITESIDE].queenBitboard  = 0b00010000;
    pieceBitboards[WHITESIDE].kingBitboard   = 0b00001000;
    //Black Starting Positions
    pieceBitboards[BLACKSIDE].pawnBitboard   = (bitboard_t)0b11111111 << 48;
    pieceBitboards[BLACKSIDE].bishopBitboard = (bitboard_t)0b00100100 << 56;
    pieceBitboards[BLACKSIDE].knightBitboard = (bitboard_t)0b01000010 << 56;
    pieceBitboards[BLACKSIDE].rookBitboard   = (bitboard_t)0b10000001 << 56;
    pieceBitboards[BLACKSIDE].queenBitboard  = (bitboard_t)0b00010000 << 56;
    pieceBitboards[BLACKSIDE].kingBitboard   = (bitboard_t)0b00001000 << 56;
}

void ChessLogic::printBoard()
{
    // occupiedBoard = pieceBitboards[WHITESIDE].bishopBitboard | pieceBitboards[WHITESIDE].knightBitboard
    //               | pieceBitboards[WHITESIDE].pawnBitboard   | pieceBitboards[WHITESIDE].rookBitboard
    //               | pieceBitboards[WHITESIDE].queenBitboard  | pieceBitboards[WHITESIDE].kingBitboard
    //               | pieceBitboards[BLACKSIDE].bishopBitboard | pieceBitboards[BLACKSIDE].knightBitboard
    //               | pieceBitboards[BLACKSIDE].pawnBitboard   | pieceBitboards[BLACKSIDE].rookBitboard
    //               | pieceBitboards[BLACKSIDE].queenBitboard  | pieceBitboards[BLACKSIDE].kingBitboard;
    int i = 0;
    for (bitboard_t position: knightPositions)
    {
        print_bitboard(position);
        std::cout << "\n\n";
    }
}

void ChessLogic::print_bitboard(bitboard_t value)
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

bitboard_t ChessLogic::getKnightBitboard(int position)
{
    if (position >= 0 && position < 64) return knightPositions[position];
    else return 0;
}

/* 
// // Beautiful function to derive knight moves
// void ChessLogic::calculateKnightMoves()
// {
//     for (int i = 64; i >= 0; i--) // Loops through all squares
//     {
//         int y = int(i/8); // Gets the column
//         int x = int(i%8); // Gets the row
//         uint64_t currentBitBoard = 0; // Temporrary bit board
//         std::array<Point,8> point = {{{-2,1},{-2,-1},{2,1},{2,-1},{1,-2},{-1,-2},{1,2},{-1,2}}}; // the knights offsets
//         for (int j=0;j<8;j++) // loops though each offset
//         {
//             if (!(x+point[j].x < 0 || x+point[j].x > 7 || y+point[j].y < 0 || y+point[j].y > 7)) // checks if the offset plus the current square is a valid square
//             {
//                 currentBitBoard |= ((uint64_t)1<<(8*(y+point[j].y))+x+point[j].x); // appends the legal kngiht offsets to the bitboard
//             }
//         }
//         knightMoveTable[i] = currentBitBoard; // sets the knight at index i equal to the current bitBoard
//     }
// }
*/