#include "ChessLogic.h"

ChessLogic::ChessLogic()
{   
    pieceBitmapLookup[CHESS_GLOBALS::PIECES::KNIGHT] = knightPositions;
    pieceBitmapLookup[CHESS_GLOBALS::PIECES::KING] = kingPositions;
    boardState[0] = CHESS_GLOBALS::PLAYER_PIECES::WHITE_KING;
    calculateMoves();

}

void ChessLogic::printBoard()
{
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
    return pieceBitmapLookup[playerPieceToPieceHash[piece]][position];
}

// Beautiful function to derive moves
 void ChessLogic::calculateMoves()
 {
    for (int i = 64; i >= 0; i--) // Loops through all squares
    {
        bitboard_t currentBoard = {0};
        int y = int(i/8); // Gets the column
        int x = int(i%8); // Gets the row
       // std::array<Point,8> point = {{{0,1},{0,-1},{1,0},{-1,0},{1,1},{-1,1},{-1,-1},{1,-1}}}; // the knights offsets
        for (int j=0;j<64;j++) // loops though each offset
        {
            int currentX = int(j%8);
            int currentY = int(j/8);
            if (((currentX == x) && (currentY != currentY)) || ((currentY == y) && (currentX != x))) // checks if the offset plus the current square is a valid square
            {
                currentBoard |= (uint64_t)1<<j; // appends the legal kngiht offsets to the bitboard
            }
        }
        print_bitboard(currentBoard);
        std:: cout << "\n" << std::endl;
        //rookPositions[i] =  currentBoard;
    }
 }
