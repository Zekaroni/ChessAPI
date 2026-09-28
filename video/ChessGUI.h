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
    int boardFontSize;
    int cursorPosition;

    Texture2D pieceTextures[PIECE_TEXTURE_COUNT];

    bitboard_t currentHightlightBitboard;
    void hightlightCurrentBitboardCells();

public:
    ChessGUI(ChessLogic* chessInstance, int screenWidth, int screenHeight);
    void highlightCursor();
    void setBoardSize(int size);
    void setBoardPostion(int x, int y); // NOTE: Top-left
    int  getBoardSize();
    Point getColumnAndRow(int index);
    void setCurrentHighlightBitboard(bitboard_t bitboard);
    void initPieceTextures();
    void renderBoard();
    void renderPieces();
    void renderFileRankText();
    void handleInputs();
    void handleMouse();
    void runGUI();
};