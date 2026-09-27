#include "ChessLogic.h"

ChessLogic::ChessLogic()
{   
    calculatePawnMoves();
    pieceBitmapLookup[CHESS_GLOBALS::PIECES::PAWN]   = pawnPositions;
    pieceBitmapLookup[CHESS_GLOBALS::PIECES::KNIGHT] = knightPositions;
    pieceBitmapLookup[CHESS_GLOBALS::PIECES::BISHOP] = bishopPositions;
    pieceBitmapLookup[CHESS_GLOBALS::PIECES::ROOK]   = rookPositions;
    pieceBitmapLookup[CHESS_GLOBALS::PIECES::QUEEN]  = queenPositions;
    pieceBitmapLookup[CHESS_GLOBALS::PIECES::KING]   = kingPositions;

    boardState[0] = CHESS_GLOBALS::PIECES::PAWN;
}

void ChessLogic::loadFEN(std::string fenString)
{
    int currentPosition = 63;
    int rank = 7;
    int file = 0;
    for(char c: fenString)
    {
        if (c == '/') rank--;
        getIndex(Point(file, rank));
    }   
}

piece_t ChessLogic::fenToPiece(char fenPiece)
{
    switch(fenPiece)
    {
        case 'P':
            return CHESS_GLOBALS::PLAYER_PIECES::WHITE_PAWN;
        case 'N':
            return CHESS_GLOBALS::PLAYER_PIECES::WHITE_KNIGHT;
        case 'B':
            return CHESS_GLOBALS::PLAYER_PIECES::WHITE_BISHOP;
        case 'R':
            return CHESS_GLOBALS::PLAYER_PIECES::WHITE_ROOK;
        case 'Q':
            return CHESS_GLOBALS::PLAYER_PIECES::WHITE_QUEEN;
        case 'K':
            return CHESS_GLOBALS::PLAYER_PIECES::WHITE_KING;
        case 'p':
            return CHESS_GLOBALS::PLAYER_PIECES::BLACK_PAWN;
        case 'n':
            return CHESS_GLOBALS::PLAYER_PIECES::BLACK_KNIGHT;
        case 'b':
            return CHESS_GLOBALS::PLAYER_PIECES::BLACK_BISHOP;
        case 'r':
            return CHESS_GLOBALS::PLAYER_PIECES::BLACK_ROOK;
        case 'q':
            return CHESS_GLOBALS::PLAYER_PIECES::BLACK_QUEEN;
        case 'k':
            return CHESS_GLOBALS::PLAYER_PIECES::BLACK_KING;
        default:
            return CHESS_GLOBALS::PIECES::EMPTY;

    }
}

void ChessLogic::print_board(bitboard_t* bitboardArray)
{
    for (int i = 0; i < sizeof(bitboardArray); i++)
    {
        std::cout << bitboardArray[i];
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

Point ChessLogic::getColumnAndRow(int index)
{
    int row = int(index/ROW_COUNT);
    int column = int(index%ROW_COUNT);
    Point columnAndRow = {column,row};
    return columnAndRow;
}

int ChessLogic::getIndex(Point pos)
{
    return (pos.y*8)+pos.x;
}

 // Beautiful function to derive moves
void ChessLogic::calculatePawnMoves()
 {
    for (int i = 63; i >= 0; i--) // Loops through all squares
    {
        bitboard_t currentBoard = {0};
        int y = int(i/8); 
        int x = int(i%8);
        for (int j=0;j<64;j++) 
        {
            int currentY = int(j/8);
            int currentX = int(j%8);
            if (currentY > y && y >= 1) // checks if the slope is 1 or -1
            {
                if (y == 1 && currentY == y+2 && currentX == x &)  
                {   
                    currentBoard |= (uint64_t)1<<j; // appends the legal square
                }
                if (y+1 <= 7 && (currentX == x || currentX == x+1 || currentX == x-1))
                {
                    currentBoard |= (uint64_t)1<<j; // appends the legal square
                }
            }
        }
        pawnPositions[i] = currentBoard;
    }
 }
