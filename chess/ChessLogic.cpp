#include "ChessLogic.h"

ChessLogic::ChessLogic()
{
    // //White Starting Positions
    // pieceBitboards[WHITESIDE].pawnBitboard   = 0b11111111 << 8;
    // pieceBitboards[WHITESIDE].bishopBitboard = 0b00100100;
    // pieceBitboards[WHITESIDE].knightBitboard = 0b01000010;
    // pieceBitboards[WHITESIDE].rookBitboard   = 0b10000001;
    // pieceBitboards[WHITESIDE].queenBitboard  = 0b00010000;
    // pieceBitboards[WHITESIDE].kingBitboard   = 0b00001000;
    // //Black Starting Positions
    // pieceBitboards[BLACKSIDE].pawnBitboard   = (bitboard_t)0b11111111 << 48;
    // pieceBitboards[BLACKSIDE].bishopBitboard = (bitboard_t)0b00100100 << 56;
    // pieceBitboards[BLACKSIDE].knightBitboard = (bitboard_t)0b01000010 << 56;
    // pieceBitboards[BLACKSIDE].rookBitboard   = (bitboard_t)0b10000001 << 56;
    // pieceBitboards[BLACKSIDE].queenBitboard  = (bitboard_t)0b00010000 << 56;
    // pieceBitboards[BLACKSIDE].kingBitboard   = (bitboard_t)0b00001000 << 56;
    pieceBitmapLookup[CHESS_GLOBALS::PIECES::KNIGHT] = knightPositions;
    // pieceBitmapLookup[CHESS_GLOBALS::PIECES::KING] = kingPositions;
    // boardState[0] = CHESS_GLOBALS::PLAYER_PIECES::WHITE_KNIGHT;
    // calculateMoves();
    // for()
    // {

    // }
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
        std::cout << position;
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

bitboard_t ChessLogic::getPiecePositionBitboard(piece_t piece, int position)
{
    return pieceBitmapLookup[2][0];
}

// Beautiful function to derive moves
 void ChessLogic::calculateMoves()
 {
    for (int i = 64; i >= 0; i--) // Loops through all squares
    {
        bitboard_t currentBoard = {0};
        int y = int(i/8); // Gets the column
        int x = int(i%8); // Gets the row
        std::array<Point,8> point = {{{0,1},{0,-1},{1,0},{-1,0},{1,1},{-1,1},{-1,-1},{1,-1}}}; // the knights offsets
        for (int j=0;j<point.size();j++) // loops though each offset
        {
            if (!(x+point[j].x < 0 || x+point[j].x > 7 || y+point[j].y < 0 || y+point[j].y > 7)) // checks if the offset plus the current square is a valid square
            {
                currentBoard |= ((uint64_t)1<<(8*(y+point[j].y))+x+point[j].x); // appends the legal kngiht offsets to the bitboard
            }
        }
        kingPositions[i] =  currentBoard;
    }
 }
