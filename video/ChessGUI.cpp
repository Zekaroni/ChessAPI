#include "ChessGUI.h"

ChessGUI::ChessGUI(int screenWidth,int screenHeight)
{
    ChessGUI::screenWidth  = screenWidth;
    ChessGUI::screenHeight = screenHeight;

    cellsPerRow = 8;
    boardSize   = 800;
    boardX      = 0;
    boardY      = 0;
    cellSize    = boardSize / cellsPerRow;
    
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

void ChessGUI::hightlightCells(bitboard_t bitboard)
{
    for (int i = 0; i < sizeof(bitboard); i++)
    {
        bitboard = bitboard >> 1;
        if (bitboard)
            DrawRectangle(
                boardX,
                boardY,
                cellSize,
                cellSize,
                sideColors[1]
            );
    }
}

void ChessGUI::runGUI()
{
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(Color{125,125,0,255});
        // All actions here
        renderBoard();

        EndDrawing();
    }
    CloseWindow();
};
