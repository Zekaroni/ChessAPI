#include "ChessGUI.h"

ChessGUI::ChessGUI(ChessLogic* chessInstance, int screenWidth,int screenHeight)
{
    ChessGUI::internalChessLogic = chessInstance;
    ChessGUI::screenWidth        = screenWidth;
    ChessGUI::screenHeight       = screenHeight;

    cellsPerRow               = 8;
    boardX                    = 0;
    boardY                    = 0;
    currentHightlightBitboard = 0;
    cursorPosition            = 0;
    
    initalize();
    setBoardSize(800); // NOTE: initalizes textures
};

void ChessGUI::initalize()
{
    SetTraceLogLevel(LOG_NONE);
    InitWindow(screenWidth, screenHeight, "Chess");
    Image windowIcon = LoadImage("./assets/images/icon.png");
    SetWindowIcon(windowIcon);
    UnloadImage(windowIcon);
}

void ChessGUI::setBoardSize(int size)
{
    ChessGUI::boardSize = size;
    cellSize = boardSize / cellsPerRow;
    boardSize = cellSize * cellsPerRow;
    boardFontSize = boardSize/(cellsPerRow*6);
    initPieceTextures();
}

void ChessGUI::setBoardPostion(int x, int y)
{
    ChessGUI::boardX = x;
    ChessGUI::boardY = y;
}

int ChessGUI::getBoardSize()
{
    return ChessGUI::boardSize;
};

void ChessGUI::renderBoard()
{
    DrawRectangle(boardX, boardY, boardSize, boardSize, *CHESS_GLOBALS::COLORS::PLAYERS[WHITESIDE]);
    for(int j = 0; j < cellsPerRow; j++)
    {
        for (int i = 0; i < cellsPerRow / 2; i++)
        {
            DrawRectangle(
                boardX + (i * cellSize * 2) + ((j % 2 == 0) ? cellSize : 0),
                boardY + (j * cellSize),
                cellSize,
                cellSize,
                *CHESS_GLOBALS::COLORS::PLAYERS[BLACKSIDE]
            );
        }
    }
};

void ChessGUI::renderFileRankText()
{
    for(int j = 0; j < cellsPerRow; j++)
    {
        DrawText(
            TextFormat("%d", cellsPerRow-j),
            boardX,
            boardY + (j * cellSize),
            boardFontSize,
            *CHESS_GLOBALS::COLORS::PLAYERS[!(j % 2)]
        );
        if (j == (cellsPerRow - 1))
        {
            for (int i = 0; i < cellsPerRow; i++)
            {
                DrawText(
                    TextFormat("%c", CHESS_GLOBALS::FILES::STRING[i]),
                    boardX + (i * cellSize) + (cellSize) - (boardFontSize),
                    boardY + (j * cellSize) + (cellSize) - (boardFontSize),
                    boardFontSize,
                    *CHESS_GLOBALS::COLORS::PLAYERS[(i % 2)]
                );
            }
        }
    }
}

void ChessGUI::renderPieces()
{
    int pieceIndex;
    Point position;
    piece_t currentPiece;
    for (int i = 63; i >= 0; i--)
    {
        currentPiece = internalChessLogic->boardState[63-i];
        if (currentPiece != 0)
        {
            position = getColumnAndRow(i);

            DrawTexture(
                pieceTextures[internalChessLogic->playerPieceToTextureIndexHash[currentPiece]],
                boardX + (cellSize * position.x),
                boardY + (cellSize * position.y),
                WHITE
            );
        }
    }
}

void ChessGUI::initPieceTextures()
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

void ChessGUI::highlightCursor()
{
    if (cursorPosition < 64)
    {
        Point cursor = getColumnAndRow(cursorPosition);
        DrawRectangle(
            boardX + ((cellsPerRow - cursor.x - 1) * cellSize),
            boardY + ((cellsPerRow - cursor.y - 1) * cellSize),
            cellSize,
            cellSize,
            CHESS_GLOBALS::COLORS::CURSOR
        );
        internalChessLogic->loggingHelper.streamToTerminal(std::to_string(cursorPosition));
    }
}

void ChessGUI::hightlightCurrentBitboardCells()
{
    if (cursorPosition < 64)
    {
        Point currentPosition;
        for (int i = 63; i >= 0; i--)
        {
            if ((currentHightlightBitboard >> i) & 1)
            {
                currentPosition = getColumnAndRow(i);
                DrawRectangle(
                    boardX + ((cellsPerRow - currentPosition.x - 1) * cellSize),
                    boardY + ((cellsPerRow - currentPosition.y - 1) * cellSize),
                    cellSize,
                    cellSize,
                    CHESS_GLOBALS::COLORS::HIGHLIGHT
                );
            }
        }
    }
}

void ChessGUI::setCurrentHighlightBitboard(bitboard_t bitboard)
{
    currentHightlightBitboard = bitboard;
}

Point ChessGUI::getColumnAndRow(int index)
{
    int row    = int(index/ROW_COUNT);
    int column = int(index%ROW_COUNT);
    Point columnAndRow = {column,row};
    return columnAndRow;
}

void ChessGUI::handleInputs()
{
    int currentKey = GetKeyPressed();    
    int tempCursorPosition = cursorPosition;
    
    switch(currentKey)
    {
        case CHESS_GLOBALS::CONTROLS::CYCLE_BITBOARD_UP:
            tempCursorPosition += 8;
        break;
        case CHESS_GLOBALS::CONTROLS::CYCLE_BITBOARD_DOWN:
            tempCursorPosition -= 8;
        break;
        case CHESS_GLOBALS::CONTROLS::CYCLE_BITBOARD_RIGHT:
            if (cursorPosition % 8 != 0) tempCursorPosition--;
        break;
        case CHESS_GLOBALS::CONTROLS::CYCLE_BITBOARD_LEFT:
            if (cursorPosition % 8 != 7) tempCursorPosition++;
        break;
    }
    if (
        tempCursorPosition >= 0 &&
        tempCursorPosition < 64 &&
        tempCursorPosition != cursorPosition
    )
    {
        cursorPosition = tempCursorPosition;
        piece_t piece = internalChessLogic->boardState[cursorPosition];
        // std::cout << (int)piece << std::endl;
        setCurrentHighlightBitboard(
            internalChessLogic->getPiecePositionBitboard(piece,cursorPosition)
        );
    }
}

void ChessGUI::handleMouse()
{
    int screenX = GetMouseX();
    int screenY = GetMouseY();

    int mouse_boardX = 0;
    int mouse_boardY = 0;

    if (screenX < boardX+boardSize && screenX >= 0+boardX && screenY < boardY+boardSize && screenY >= 0+boardY)
    {
        mouse_boardX = ((screenX-boardX)/cellSize)+1;
        mouse_boardY = (screenY-boardY)/cellSize;

        cursorPosition = internalChessLogic->getIndex({mouse_boardX,mouse_boardY});

        piece_t piece = internalChessLogic->boardState[cursorPosition];
        setCurrentHighlightBitboard(
            internalChessLogic->getLegalMovesBitboard(piece,cursorPosition)
        );
    } else
    {
        cursorPosition = 64;
    }
}

void ChessGUI::runGUI()
{
    while (!WindowShouldClose())
    {
        handleInputs();
        handleMouse();

        BeginDrawing();
        ClearBackground(CHESS_GLOBALS::COLORS::BACKGROUND);
        renderBoard();
        renderFileRankText();
        
        highlightCursor();
        hightlightCurrentBitboardCells();
        
        renderPieces();
        
        EndDrawing();
    }
    CloseWindow();
};