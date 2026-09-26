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
    calculateMoves();
    for (bitboard_t position: kingPositions)
    {
        print_bitboard(position);
        std::cout << "\n\n";
    }
    pieceBitmapLookup[CHESS_GLOBALS::PIECES::KNIGHT] = knightPositions;
    pieceBitmapLookup[CHESS_GLOBALS::PIECES::BISHOP] = bishopPositions;
    pieceBitmapLookup[CHESS_GLOBALS::PIECES::ROOK] = rookPositions;
    pieceBitmapLookup[CHESS_GLOBALS::PIECES::QUEEN] = queenPositions;
    pieceBitmapLookup[CHESS_GLOBALS::PIECES::KING] = kingPositions;

    boardState[0] = CHESS_GLOBALS::PIECES::QUEEN;
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
    for (int i = 0; i <64; i++) // Loops through all squares
    {
        bitboard_t currentBoard = {0};
        currentBoard = bishopPositions[i] | rookPositions[i];
        //queenPositions[i] =  currentBoard;
        std::cout << currentBoard << "ull,\n";
    }
}
