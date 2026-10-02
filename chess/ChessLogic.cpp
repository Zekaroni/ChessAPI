#include "ChessLogic.h"

ChessLogic::ChessLogic()
{
    loadFEN(CHESS_GLOBALS::STARTING_FEN_STRING);
    currentPlayer = WHITESIDE;
    loggingHelper.debug_pieces(boardState);
    calculateEdge();
    getRookBlockerBitBoards(28);
    //generateRookMoveTable();
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
                legalMoves = whitePawnPositions[position] &
                            ~occupiedBitboards[0]
                ;
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
            ;
        break;
        case CHESS_GLOBALS::PIECES::ROOK:
            legalMoves = rookLegalMoveConfigurations[200];
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


//Magic Bitboards function
void ChessLogic::generateRookMoveTable()
{
    for (int i = 0;i < 64;i++)
    {
        //getRookBlockerBitBoards(i);
    }
}


void ChessLogic::getRookBlockerBitBoards(int index)
{
    Point pos = getFileAndRank(index);
    bitboard_t rookWithOutEdge = rookPositions[index];
    if (pos.x != 0 && pos.x != 7)
    {
        rookWithOutEdge = rookWithOutEdge&~(rookPositions[index]&edgeBitBoard[0]);
    }
    if (pos.y != 0 && pos.y != 7)
    {
        rookWithOutEdge = rookWithOutEdge&~(rookPositions[index]&edgeBitBoard[1]);
    }
    int counter = 0;
    for (bitboard_t rookBlockerConfigurations = rookWithOutEdge;rookBlockerConfigurations;rookBlockerConfigurations=(rookBlockerConfigurations-1)&rookWithOutEdge)
    {
        bitboard_t currentBitboard = 0;
        //dir
        for (int topDir = 1;topDir < 8;topDir++)
        {
            bitboard_t mask = (bitboard_t)1<<index+topDir*8;
            if (getFileAndRank(index+topDir).y > 7){break;}
            if ((mask&rookBlockerConfigurations)!=0)
            {
                break;
            }else
            {
                currentBitboard |= mask;
            }
        }
        for (int bottomDir = 1;bottomDir < 8;bottomDir++)
        {
            bitboard_t mask = (bitboard_t)1<<index-bottomDir*8;
            if (getFileAndRank(index+bottomDir).y <= 0){break;}
            if ((mask&rookBlockerConfigurations)!=0)
            {
                break;
            }else
            {
                currentBitboard |= mask;
            }
        }
        for (int leftDir = 1;leftDir < 8;leftDir++)
        {
            bitboard_t mask = (bitboard_t)1<<(index+leftDir);
            if (getFileAndRank(index+leftDir).x >= 7){break;}
            if ((mask&rookBlockerConfigurations)!=0)
            {
                break;
            }else
            {
                currentBitboard |= mask;
            }
        }
        for (int rightDir = 1;rightDir < 8;rightDir++)
        {
            if (getFileAndRank(index-rightDir).x <= 0){break;}
            bitboard_t mask = (bitboard_t)1<<(index-rightDir);
            if ((mask&rookBlockerConfigurations)!=0)
            {
                break;
            }else
            {
                currentBitboard |= mask;
            }
        }
        rookLegalMoveConfigurations.push_back(currentBitboard);
    }
}

// Beautiful function to derive moves
 void ChessLogic::calculateEdge()
 {
   // for (int i = 63; i >= 0; i--) // Loops through all squares
   // {
        bitboard_t currentBoardX = {0};
        bitboard_t currentBoardY = {0};
       // int y = int(i/8); 
       // int x = int(i%8);
        for (int j=0;j<64;j++) 
        {
            int currentY = int(j/8);
            int currentX = int(j%8);
           // if (currentY < y && y <= 6) // checks if the slope is 1 or -1
           // {
                if(currentX == 0 || currentX == 7)
                {
                    currentBoardX |= (uint64_t)1<<j;
                }
                if (currentY == 0 || currentY == 7)
                {
                    currentBoardY |= (uint64_t)1<<j;
                }
           // }
        }
        // std::cout << currentBoard << "ULL," <<std::endl;
        edgeBitBoard[0] = currentBoardX;
        edgeBitBoard[1] = currentBoardY;
    //}
 }