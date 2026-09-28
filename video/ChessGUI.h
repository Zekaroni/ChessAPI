#include "../include/utils.h"
#include "../chess/ChessLogic.cpp"

// NOTE: Everthing is top-left oriented
class ChessBoard
{
public: // public FOR NOW
    ChessLogic* internalChessLogic = nullptr;
    int boardSize = 0;
    int boardX;
    int boardY;
    int cellsPerRow;
    int cellSize;
    int boardFontSize;
    int cursorPosition;
    Texture2D pieceTextures[PIECE_TEXTURE_COUNT] = {};
    bitboard_t currentHightlightBitboard;
    
public:
    ChessBoard(ChessLogic* chessInstance, int boardX, int boardY, int boardSize);
    
    // Setters
    void setBoardSize(int size);
    void setBoardPostion(int x, int y); // NOTE: Top-left
    void setCurrentHighlightBitboard(bitboard_t bitboard);
    
    // Getters
    int  getBoardSize();

    void initPieceTextures(); // Stays in this class because each one has separte textures
};


class ChessGUI
{
private:
    int screenWidth;
    int screenHeight;
    std::vector<ChessBoard*> boards;
    
public:
    ChessGUI(int screenWidth, int screenHeight);
    Point getColumnAndRow(int index); // May move to GUI
    void initalize();

    // Add a board to the GUI
    void addBoard(ChessBoard& board);

    // Input
    void handleInputs(ChessBoard& board);
    void handleMouse(ChessBoard& board);


    // NOTE: The idea is there can be multiple boards that all have different
    //       states and we can render them individually

    // Render Methods
    void renderBoard(ChessBoard& board);
    void renderPieces(ChessBoard& board);
    // Highlight and Overlays
    void highlightCursor(ChessBoard& board);
    void hightlightCurrentBitboardCells(ChessBoard& board);
    void renderFileRankText(ChessBoard& board);

    void runGUI();
};