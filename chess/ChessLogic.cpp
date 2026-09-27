#include "ChessLogic.h"

ChessLogic::ChessLogic()
{
    loadFEN(CHESS_GLOBALS::STARTING_FEN_STRING);
    currentPlayer = WHITESIDE;

    pieceBitmapLookup[CHESS_GLOBALS::PIECES::PAWN]   = pawnPositions;
    pieceBitmapLookup[CHESS_GLOBALS::PIECES::KNIGHT] = knightPositions;
    pieceBitmapLookup[CHESS_GLOBALS::PIECES::BISHOP] = bishopPositions;
    pieceBitmapLookup[CHESS_GLOBALS::PIECES::ROOK]   = rookPositions;
    pieceBitmapLookup[CHESS_GLOBALS::PIECES::QUEEN]  = queenPositions;
    pieceBitmapLookup[CHESS_GLOBALS::PIECES::KING]   = kingPositions;
    
    // boardState[0] = CHESS_GLOBALS::PLAYER_PIECES::WHITE_PAWN;
    // print_board(pawnPositions, 64);

    print_pieces_at_position();
}

void ChessLogic::loadFEN(std::string fenString)
{
    int rank = 7;
    int file = 0;
    for(int i = 0; i < fenString.length(); i++)
    {
        if (fenString[i] == '/')
        {
            rank--;
            file = 0;
        } else if ((fenString[i] < 122) && (fenString[i] > 64))
        {
            printf("%d", file);
            boardState[getIndex({file, rank})] = fenToPiece(fenString[i]);
            file++;
        } else if ((fenString[i] < 58) && (fenString[i] > 48))
        {
            file += (fenString[i] - 48);
        } else if (fenString[i] == ' ')
        {
            currentPlayer = fenString[i+1] == 'b';
            break;
        }
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
    return (64 - ((pos.y * 8) + pos.x));
}

// Beautiful function to derive moves
void ChessLogic::calculatePawnMoves()
{
    int j = 63;
    for (int i = 63; i >= 0; i--) // Loops through all squares
    {
        bitboard_t currentBoard = {0};
        int y = int(i/8); 
        int x = int(i%8);
        blackPawnPosition[i] = pawnPositions[j];
    }
}

void ChessLogic::print_pieces_at_position()
{
    for (int i = 0; i <= 64; i++)
    {
        printf("%d  ", boardState[i]);
        if (i % 8 == 0) printf("\n");
    }
}

void ChessLogic::print_board(bitboard_t* bitboardArray,int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << bitboardArray[i];
        std::cout << "\n";
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