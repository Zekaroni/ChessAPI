#include "../include/utils.h"
#include "../chess/ChessLogic.cpp"

// NOTE: Everthing is top-left oriented
class ChessGUI
{
private:
    ChessLogic* internalChessLogic;
    int screenWidth;
    int screenHeight;
    int cellsPerRow;

    int boardSize;
    int boardX;
    int boardY;
    int cellSize;
    int cursorPosition;

    bitboard_t currentHightlightBitboard;
    Color sideColors[PLAYER_COUNT] = {CHESS_GLOBALS::COLORS::WHITE_SIDE, CHESS_GLOBALS::COLORS::BLACK_SIDE};
    void hightlightCells();

public:
    ChessGUI(ChessLogic* chessInstance, int screenWidth, int screenHeight);
    void setBoardSize(int size);
    void setBoardPostion(int x, int y); // NOTE: Top-left
    int  getBoardSize();
    Point getColumnAndRow(int index);
    void setCurrentHighlightBitboard(bitboard_t bitboard);
    void renderBoard();
    void handleInputs();
    void runGUI();
};