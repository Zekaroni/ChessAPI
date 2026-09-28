#include "ChessLogic.h"

ChessLogic::ChessLogic()
{
    // calculateBlackPawnMoves();
    loadFEN(CHESS_GLOBALS::STARTING_FEN_STRING);
    currentPlayer = WHITESIDE;
    
    // boardState[0] = CHESS_GLOBALS::PLAYER_PIECES::WHITE_PAWN;
    // print_board(blackPawnPositions, 64);
    // print_pieces_at_position();
}

void ChessLogic::loadFEN(std::string fenString)
{
    std::fill(std::begin(boardState), std::end(boardState), piece_t{});
    int currentBoardPosition = 63;
    int rank = 7;
    int file = 0;
    for(int i = 0; i < fenString.length(); i++)
    {
        if (fenString[i] == '/')
        {
            rank--;
            currentBoardPosition = rank * 8 + 7;
            file = 0;
        } else if ((fenString[i] < 122) && (fenString[i] > 64))
        {
            boardState[currentBoardPosition] = fenToPiece(fenString[i]);
            file++;
            currentBoardPosition--;
        } else if ((fenString[i] < 58) && (fenString[i] > 48))
        {
            int offset = (fenString[i] - 48) ;
            file += offset;
            currentBoardPosition -= offset;
        } else if (fenString[i] == ' ')
        {
            currentPlayer = fenString[i+1] == 'b';
            break;
        }
    }  
    occupiedBitboards[0] = 0;
    occupiedBitboards[1] = 0;
    generateBlackAndWhiteOccupiedBitboards();
}

void ChessLogic::generateBlackAndWhiteOccupiedBitboards()
{
    piece_t currentPiece = 0;
    for (int i = 63; i >= 0 ; i--)
    {
        currentPiece = boardState[i];
        if(boardState[i])
        {
            occupiedBitboards[playerPieceToPlayerHash[currentPiece]] |= (bitboard_t)1 << i;
        }
    }
    print_bitboard(occupiedBitboards[0]);
    std::cout << "\n";
    print_bitboard(occupiedBitboards[1]);
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
    // if (piece == 0b1001 || piece == 0b0001)
    // {
    //     return (&(pieceBitmapLookup[playerPieceToPieceHash[piece]])[piece & 0b100])[position];
    // } else {
    //     return pieceBitmapLookup[playerPieceToPieceHash[piece]][position];
    // }
    switch(piece)
    {
        case CHESS_GLOBALS::PLAYER_PIECES::WHITE_PAWN:
            return whitePawnPositions[position];
        break;
        case CHESS_GLOBALS::PLAYER_PIECES::WHITE_KNIGHT:
            return knightPositions[position];
        break;
        case CHESS_GLOBALS::PLAYER_PIECES::WHITE_BISHOP:
            return bishopPositions[position];
        break;
        case CHESS_GLOBALS::PLAYER_PIECES::WHITE_ROOK:
            return rookPositions[position];
        break;
        case CHESS_GLOBALS::PLAYER_PIECES::WHITE_QUEEN:
            return queenPositions[position];
        break;
        case CHESS_GLOBALS::PLAYER_PIECES::WHITE_KING:
            return kingPositions[position];
        break;
        case CHESS_GLOBALS::PLAYER_PIECES::BLACK_PAWN:
            return blackPawnPositions[position];
        break;
        case CHESS_GLOBALS::PLAYER_PIECES::BLACK_KNIGHT:
            return knightPositions[position];
        break;
        case CHESS_GLOBALS::PLAYER_PIECES::BLACK_BISHOP:
            return bishopPositions[position];
        break;
        case CHESS_GLOBALS::PLAYER_PIECES::BLACK_ROOK:
            return rookPositions[position];
        break;
        case CHESS_GLOBALS::PLAYER_PIECES::BLACK_QUEEN:
            return queenPositions[position];
        break;
        case CHESS_GLOBALS::PLAYER_PIECES::BLACK_KING:
            return kingPositions[position];
        break;
        default:
            return CHESS_GLOBALS::PIECES::EMPTY;
    }
}


Point ChessLogic::getFileAndRank(int index)
{
    int rank = 8 - int(index/ROW_COUNT);
    int file = int(index%ROW_COUNT);
    Point fileAndRank = {file,rank};
    return fileAndRank;
}

int ChessLogic::getIndex(Point pos)
{
    return (64 - ((pos.y * 8) + pos.x));
}

bitboard_t ChessLogic::getLegalMovesBitboard(piece_t piece,int position)
{
    bitboard_t legalMoves = {0};
    bitboard_t occupiedBitboard = occupiedBitboards[0] | occupiedBitboards [1];
    switch (playerPieceToPieceHash[piece])
    {
        case CHESS_GLOBALS::PIECES::PAWN:
            if (piece == CHESS_GLOBALS::PLAYER_PIECES::WHITE_PAWN)
            {
                // for white
                legalMoves = whitePawnPositions[position];
            } else {
                legalMoves = blackPawnPositions[position];
                // for black
                //legalMoves = blackPawnPositions[position]&occupiedBitboards[1]; 
            }
        break;
        case CHESS_GLOBALS::PIECES::KNIGHT:
            legalMoves = knightPositions[position] & ~occupiedBitboards[playerPieceToPlayerHash[piece]];
        break;
        case CHESS_GLOBALS::PIECES::BISHOP:
            legalMoves = bishopPositions[position];
        break;
        case CHESS_GLOBALS::PIECES::ROOK:
            legalMoves = rookPositions[position];
        break;
        case CHESS_GLOBALS::PIECES::QUEEN:
            legalMoves = queenPositions[position];
        break;
        case CHESS_GLOBALS::PIECES::KING:
            legalMoves = kingPositions[position]&
                         ~occupiedBitboards[playerPieceToPlayerHash[piece]];
        break;
        default:
            break;
    }
    attackingSquares[playerPieceToPlayerHash[piece]] |= legalMoves;
    return legalMoves;
} 


// Beautiful function to derive moves
 void ChessLogic::calculateBlackPawnMoves()
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
            if (currentY < y && y <= 6) // checks if the slope is 1 or -1
            {
                if (y == 6 && currentY == 6-2 && currentX == x)  
                {   
                    currentBoard |= (uint64_t)1<<j; // appends the legal square
                }
                if (y-1 <= 7 && currentY == y-1 &&  (currentX == x || currentX == x+1 || currentX == x-1))
                {
                    currentBoard |= (uint64_t)1<<j; // appends the legal square
                }
            }
        }
        // std::cout << currentBoard << "ULL," <<std::endl;
        // blackPawnPositions[i] = currentBoard;
    }
 }

void ChessLogic::print_pieces_at_position()
{
    for (int i = 63; i > 0; i--)
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