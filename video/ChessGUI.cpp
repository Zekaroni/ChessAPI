#include "ChessGUI.h"

ChessGUI::ChessGUI(ChessLogic* chessInstance, int screenWidth,int screenHeight)
{
    ChessGUI::internalChessLogic = chessInstance;
    ChessGUI::screenWidth        = screenWidth;
    ChessGUI::screenHeight       = screenHeight;

    cellsPerRow               = 8;
    boardSize                 = 800;
    boardX                    = 0;
    boardY                    = 0;
    cellSize                  = boardSize / cellsPerRow;
    currentHightlightBitboard = 0;
    cursorPosition            = 0;
    boardFontSize             = boardSize/(cellsPerRow*2);
    
    SetTraceLogLevel(LOG_NONE);
    InitWindow(screenWidth, screenHeight, "Chess");
};

void ChessGUI::setBoardSize(int size)
{
    ChessGUI::boardSize = size;
    cellSize = boardSize / cellsPerRow;
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
    DrawRectangle(boardX, boardY, boardSize, boardSize, *CHESS_GLOBALS::COLORS::PLAYERS[0]);
    for(int j = 0; j < cellsPerRow; j++)
    {
        for (int i = 0; i < cellsPerRow / 2; i++)
        {
            DrawRectangle(
                boardX + (i * cellSize * 2) + ((j % 2 == 0) ? cellSize : 0),
                boardY + (j * cellSize),
                cellSize,
                cellSize,
                *CHESS_GLOBALS::COLORS::PLAYERS[1]
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
                    boardX + (i * cellSize) + (boardFontSize),
                    boardY + (j * cellSize) + (boardFontSize),
                    boardFontSize,
                    *CHESS_GLOBALS::COLORS::PLAYERS[(i % 2)]
                );
            }
        }
    }
}

void ChessGUI::highlightCursor()
{
    Point cursor = internalChessLogic->getColumnAndRow(cursorPosition);
    DrawRectangle(
        boardX + ((cellsPerRow - cursor.x - 1) * cellSize),
        boardY + ((cellsPerRow - cursor.y - 1) * cellSize),
        cellSize,
        cellSize,
        CHESS_GLOBALS::COLORS::CURSOR
    );
}

void ChessGUI::hightlightCells()
{
    Point currentPosition;
    for (int i = 63; i >= 0; i--)
    {
        if ((currentHightlightBitboard >> i) & 1)
        {
            currentPosition = internalChessLogic->getColumnAndRow(i);
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

void ChessGUI::setCurrentHighlightBitboard(bitboard_t bitboard)
{
    currentHightlightBitboard = bitboard;
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
        std::cout << (int)piece << std::endl;
        setCurrentHighlightBitboard(
            internalChessLogic->getPiecePositionBitboard(piece,cursorPosition)
        );
    }
}

void ChessGUI::runGUI()
{
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(CHESS_GLOBALS::COLORS::BACKGROUND);
        renderBoard();
        renderFileRankText();
        
        highlightCursor();
        hightlightCells();
        handleInputs();

        EndDrawing();
    }
    CloseWindow();
};