#include "ChessGUI.h"

ChessGUI::ChessGUI(ChessLogic chessInstance, int screenWidth,int screenHeight)
{
    ChessGUI::internalChessLogic = chessInstance;

    ChessGUI::screenWidth  = screenWidth;
    ChessGUI::screenHeight = screenHeight;

    cellsPerRow = 8;
    boardSize   = 800;
    boardX      = 0;
    boardY      = 0;
    cellSize    = boardSize / cellsPerRow;
    currentHightlightBitboard = 0;
    
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
    DrawRectangle(boardX, boardY, boardSize, boardSize, sideColors[0]);
    for(int j = 0; j < cellsPerRow; j++)
    {
        for (int i = 0; i < cellsPerRow / 2; i++)
        {
            DrawRectangle(
                boardX + (i * cellSize * 2) + ((j % 2 == 0) ? cellSize : 0),
                boardY + (j * cellSize),
                cellSize,
                cellSize,
                sideColors[1]
            );
        }
    }
};

void ChessGUI::hightlightCells()
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
                CHESS_COLORS::HIGHLIGHT
            );
        }
    }
}

void ChessGUI::setCurrentHighlightBitboard(bitboard_t bitboard)
{
    currentHightlightBitboard = bitboard;
}

void ChessGUI::runGUI()
{
    int currentBitboardPosition = 0;
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            currentHightlightBitboard = internalChessLogic.getKnightBitboard(currentBitboardPosition);
            currentBitboardPosition++;
            if (currentBitboardPosition > 63) currentBitboardPosition = 0;
        }
        BeginDrawing();
        ClearBackground(Color{125,125,0,255});
        renderBoard();
        if (currentHightlightBitboard)
        {
            hightlightCells();
        }
        EndDrawing();
    }
    CloseWindow();
};

Point ChessGUI::getColumnAndRow(int index)
{
    int row = int(index/cellsPerRow);
    int column = int(index%cellsPerRow);
    Point columnAndRow = {column,row};
    return columnAndRow;
}
