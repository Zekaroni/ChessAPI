#include "../include/utils.h"

// NOTE: Everthing is top-left oriented
class ChessGUI
{
private:
    int screenWidth;
    int screenHeight;
    int cellsPerRow;

    int boardSize;
    int boardX;
    int boardY;
    int cellSize;
    Color sideColors[2] = {{255,255,255,255},{0,0,0,255}};

public:
    ChessGUI(int screenWidth, int screenHeight);
    void setBoardSize(int size);
    void setBoardPostion(int x, int y); // NOTE: Top-left
    int  getBoardSize();
    void getColumnAndRow();
    void hightlightCells(bitboard_t bitboard);
    void renderBoard();
    void runGUI();
};