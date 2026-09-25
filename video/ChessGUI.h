#include "../include/utils.h"
#include "../chess/ChessLogic.cpp"

// NOTE: Everthing is top-left oriented
class ChessGUI
{
private:
    ChessLogic internalChessLogic;
    int screenWidth;
    int screenHeight;
    int cellsPerRow;

    int boardSize;
    int boardX;
    int boardY;
    int cellSize;

    bitboard_t currentHightlightBitboard;
    Color sideColors[2] = {{255,255,255,255},{0,0,0,255}};
    void hightlightCells();

public:
    ChessGUI(ChessLogic chessInstance, int screenWidth, int screenHeight);
    void setBoardSize(int size);
    void setBoardPostion(int x, int y); // NOTE: Top-left
    int  getBoardSize();
    Point getColumnAndRow(int index);
    void setCurrentHighlightBitboard(bitboard_t bitboard);
    void renderBoard();
    void runGUI();
};