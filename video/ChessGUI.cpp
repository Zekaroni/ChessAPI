#include "ChessGUI.h"

// ChessBoard Class Methods

ChessBoard::ChessBoard(ChessLogic* chessInstance, int boardX, int boardY, int boardSize)
{
    internalChessLogic        = chessInstance;
    ChessBoard::boardSize     = boardSize;
    ChessBoard::boardX        = boardX;
    ChessBoard::boardY        = boardY;
    cellsPerRow               = 8;
    cellSize                  = boardSize / 8;
    ChessBoard::boardSize     = cellSize * 8;
    boardFontSize             = boardSize/(cellSize * 3);
    cursorPosition            = 0;
    currentHightlightBitboard = (bitboard_t)0;

    initPieceTextures();
};

void ChessBoard::setBoardSize(int size)
{
    ChessBoard::boardSize = size;
    cellSize = boardSize / cellsPerRow;
    boardSize = cellSize * cellsPerRow;
    boardFontSize = boardSize/(cellsPerRow*6);
    initPieceTextures();
}


// Setters

void ChessBoard::setBoardPostion(int x, int y)
{
    ChessBoard::boardX = x;
    ChessBoard::boardY = y;
}

// Getters

int ChessBoard::getBoardSize()
{
    return ChessBoard::boardSize;
};

void ChessBoard::initPieceTextures()
{
    Image img;
    std::string pathString;
    bool unloadTextures = pieceTextures[0].id > 0;
    
    for (int i = 0; i < PIECE_TEXTURE_COUNT/2;i++)
    {
        if (unloadTextures)
        {
            UnloadTexture(pieceTextures[i]);
            UnloadTexture(pieceTextures[i+6]);
            pieceTextures[i] = {};
            pieceTextures[i+6] = {};
        }
        pathString = std::string("./assets/images/") + CHESS_GLOBALS::INDEX_TO_FEN_LETTER[i+1] + ".png";
        img = LoadImage(pathString.c_str());
        ImageResize  (&img,cellSize,cellSize);          // scale to board
        pieceTextures[i+6] = LoadTextureFromImage(img); // black pieces
        
        ImageColorInvert(&img);  // for white pieces
        pieceTextures[i] = LoadTextureFromImage(img);
        
        UnloadImage(img);
    }
}

void ChessBoard::setCurrentHighlightBitboard(bitboard_t bitboard)
{
    currentHightlightBitboard = bitboard;
}


// ChessGUI Methods

ChessGUI::ChessGUI(int screenWidth,int screenHeight)
{
    ChessGUI::screenWidth        = screenWidth;
    ChessGUI::screenHeight       = screenHeight;
    
    initalize();
};

void ChessGUI::addBoard(ChessBoard& board)
{
    boards.push_back(&board);
}

Point ChessGUI::getColumnAndRow(int index)
{
    int row    = int(index / ROW_COUNT);
    int column = int(index % ROW_COUNT);
    Point columnAndRow = {column,row};
    return columnAndRow;
}

// Render Methods
// NOTE: I tried to order them in order of least to greatest "layer"
//       meaning the first one is the first to render and the next
//       will render over it.

void ChessGUI::renderBoard(ChessBoard& board)
{
    DrawRectangle(board.boardX, board.boardY, board.boardSize, board.boardSize, *CHESS_GLOBALS::COLORS::PLAYERS[WHITESIDE]);
    for(int j = 0; j < board.cellsPerRow; j++)
    {
        for (int i = 0; i < board.cellsPerRow / 2; i++)
        {
            DrawRectangle(
                board.boardX + (i * board.cellSize * 2) + ((j % 2 == 0) ? board.cellSize : 0),
                board.boardY + (j * board.cellSize),
                board.cellSize,
                board.cellSize,
                *CHESS_GLOBALS::COLORS::PLAYERS[BLACKSIDE]
            );
        }
    }
};

void ChessGUI::renderFileRankText(ChessBoard& board)
{
    for(int j = 0; j < board.cellsPerRow; j++)
    {
        DrawText(
            TextFormat("%d", board.cellsPerRow-j),
            board.boardX,
            board.boardY + (j * board.cellSize),
            board.boardFontSize,
            *CHESS_GLOBALS::COLORS::PLAYERS[!(j % 2)]
        );
        if (j == (board.cellsPerRow - 1))
        {
            for (int i = 0; i < board.cellsPerRow; i++)
            {
                DrawText(
                    TextFormat("%c", CHESS_GLOBALS::FILES::STRING[i]),
                    board.boardX + (i * board.cellSize) + (board.cellSize) - (board.boardFontSize),
                    board.boardY + (j * board.cellSize) + (board.cellSize) - (board.boardFontSize),
                    board.boardFontSize,
                    *CHESS_GLOBALS::COLORS::PLAYERS[(i % 2)]
                );
            }
        }
    }
}

void ChessGUI::renderPieces(ChessBoard& board)
{
    int pieceIndex;
    Point position;
    piece_t currentPiece;
    for (int i = 63; i >= 0; i--)
    {
        currentPiece = board.internalChessLogic->boardState[63-i];
        if (currentPiece != 0)
        {
            position = getColumnAndRow(i);
            
            DrawTexture(
                board.pieceTextures[board.internalChessLogic->playerPieceToTextureIndexHash[currentPiece]],
                board.boardX + (board.cellSize * position.x),
                board.boardY + (board.cellSize * position.y),
                WHITE
            );
        }
    }
}

void ChessGUI::highlightCursor(ChessBoard& board)
{
    if (board.cursorPosition < 64)
    {
        Point cursor = getColumnAndRow(board.cursorPosition);
        DrawRectangle(
            board.boardX + ((board.cellsPerRow - cursor.x - 1) * board.cellSize),
            board.boardY + ((board.cellsPerRow - cursor.y - 1) * board.cellSize),
            board.cellSize,
            board.cellSize,
            CHESS_GLOBALS::COLORS::CURSOR
        );
        board.internalChessLogic->loggingHelper.streamToTerminal(std::to_string(board.cursorPosition) + " ");
    }
}

void ChessGUI::hightlightCurrentBitboardCells(ChessBoard& board)
{
    if (board.cursorPosition < 64)
    {
        Point currentPosition;
        for (int i = 63; i >= 0; i--)
        {
            if ((board.currentHightlightBitboard >> i) & 1)
            {
                currentPosition = getColumnAndRow(i);
                DrawRectangle(
                    board.boardX + ((board.cellsPerRow - currentPosition.x - 1) * board.cellSize),
                    board.boardY + ((board.cellsPerRow - currentPosition.y - 1) * board.cellSize),
                    board.cellSize,
                    board.cellSize,
                    CHESS_GLOBALS::COLORS::HIGHLIGHT
                );
            }
        }
    }
}

void ChessGUI::initalize()
{
    SetTraceLogLevel(LOG_NONE);
    InitWindow(screenWidth, screenHeight, "Chess");
    Image windowIcon = LoadImage("./assets/images/icon.png");
    SetWindowIcon(windowIcon);
    UnloadImage(windowIcon);
}

void ChessGUI::handleInputs(ChessBoard& board)
{
    int currentKey = GetKeyPressed();    
    int tempCursorPosition = board.cursorPosition;
    
    switch(currentKey)
    {
        case CHESS_GLOBALS::CONTROLS::CYCLE_BITBOARD_UP:
        tempCursorPosition += 8;
        break;
        case CHESS_GLOBALS::CONTROLS::CYCLE_BITBOARD_DOWN:
            tempCursorPosition -= 8;
        break;
        case CHESS_GLOBALS::CONTROLS::CYCLE_BITBOARD_RIGHT:
        if (board.cursorPosition % 8 != 0) tempCursorPosition--;
        break;
        case CHESS_GLOBALS::CONTROLS::CYCLE_BITBOARD_LEFT:
        if (board.cursorPosition % 8 != 7) tempCursorPosition++;
        break;
    }
    if (
        tempCursorPosition >= 0 &&
        tempCursorPosition < 64 &&
        tempCursorPosition != board.cursorPosition
    )
    {
        board.cursorPosition = tempCursorPosition;
        piece_t piece = board.internalChessLogic->boardState[board.cursorPosition];
        // std::cout << (int)piece << std::endl;
        board.setCurrentHighlightBitboard(
            board.internalChessLogic->getPiecePositionBitboard(piece,board.cursorPosition)
        );
    }
}
   
void ChessGUI::handleMouse(ChessBoard& board)
{
    int screenX = GetMouseX();
    int screenY = GetMouseY();
    
    int mouse_boardX = 0;
    int mouse_boardY = 0;
    
    if (screenX < board.boardX + board.boardSize && screenX >= 0 + board.boardX &&
        screenY < board.boardY + board.boardSize && screenY >= 0 + board.boardY)
    {
        mouse_boardX = ((screenX-board.boardX) / board.cellSize) + 1;
        mouse_boardY = (screenY-board.boardY) / board.cellSize;
        
        board.cursorPosition = board.internalChessLogic->getIndex({mouse_boardX,mouse_boardY});
        
        piece_t piece = board.internalChessLogic->boardState[board.cursorPosition];
        board.setCurrentHighlightBitboard(
            board.internalChessLogic->getLegalMovesBitboard(piece,board.cursorPosition)
        );
    } else
    {
        board.cursorPosition = 64;
    }
}

void ChessGUI::runGUI()
{
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(CHESS_GLOBALS::COLORS::BACKGROUND);

        if (!boards.empty())
        {
            for (ChessBoard* board: boards)
            {
                handleInputs(*board);
                handleMouse(*board);
                
                renderBoard(*board);
                renderFileRankText(*board);
                
                highlightCursor(*board);
                hightlightCurrentBitboardCells(*board);
                
                renderPieces(*board);
            }
        }
        
        EndDrawing();
    }
    CloseWindow();
};