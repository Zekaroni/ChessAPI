#include "../include/utils.h"
#include "../chess/ChessLogic.cpp"

// NOTE: Everthing is top-left oriented for the GUI
class ChessBoard
{
private:
    int _boardSize = 0;
    int _boardX;
    int _boardY;
    int _cellsPerRow;
    int _cellSize;
    int _boardFontSize;
    int _cursorPosition;
    ChessLogic* _internalChessLogic = nullptr;
    bitboard_t  _currentHighlightBitboard;

public:
    ChessBoard(ChessLogic* chessInstance, int boardX, int boardY, int boardSize);

    //---// Setters

    void setBoardSize(int size);
    void setBoardPostion(int x, int y); // NOTE: Top-left
    void setCurrentHighlightBitboard(bitboard_t bitboard){_currentHighlightBitboard = bitboard;};
    void boardSize(int size);
    void boardX(int x)               { _boardX         = x; };
    void boardY(int y)               { _boardY         = y; };
    void cellsPerRow(int cellsPerRow){ _cellsPerRow    = cellsPerRow; };
    void cellSize(int size)          { _cellSize       = size; };
    void boardFontSize(int fontSize) { _boardFontSize  = fontSize; };
    void cursorPosition(int position){ _cursorPosition = position; };
    

    //---// Getters

    int boardSize()      const { return _boardSize; };
    int boardX()         const { return _boardX; };
    int boardY()         const { return _boardY; };
    int cellsPerRow()    const { return _cellsPerRow; };
    int cellSize()       const { return _cellSize; };
    int boardFontSize()  const { return _boardFontSize; };
    int cursorPosition() const { return _cursorPosition; };
    ChessLogic* internalChessLogic() const { return _internalChessLogic; };
    bitboard_t currentHighlightBitboard() const { return _currentHighlightBitboard; }
};


class ChessGUI
{
private:
    // NOTE: Storing the baked images in GUI because it isn't
    // really any part of the logic.
    static constexpr uint64_t _bakedChessPieceImages[6][32] = {
        {0},
        {0},
        {0},
        {0},
        {0},
        {0}
    };    
    static constexpr uint8_t playerPieceToTextureIndexHash [16] = {
        255, 0, 1, 2, 3, 4, 5, 255, 255, 6, 7, 8, 9, 10, 11, 255
    };
    int _screenWidth;
    int _screenHeight;
    int _biggestDimesion;
    int _maxBoardSize;
    int _maxCellSize;
    std::vector<ChessBoard*> _boards;
    std::vector<ChessLogic*> __chessLogicMemorySpace;
    bool _hasChange;
    RenderTexture2D _boardFrameCache;
    RenderTexture2D _boardTextureCache;
    Texture2D   _pieceTextures[PIECE_TEXTURE_COUNT] = {};
    
    uint64_t _totalFrames = 0; // for debug purposes
    
    ///---// Internal Methods

    void __initalize();

    // Texture caching
    void _cacheBoardTexture();
    void _cachePieceTextures();
    
    // Render Methods
    void _renderBoard(ChessBoard& board);
    void _renderPieces(ChessBoard& board);
    void _renderFullGUICache();

    // Highlight and Overlays
    void _highlightCursor(ChessBoard& board);
    void _hightlightCurrentBitboardCells(ChessBoard& board);
    void _renderFileRankText(ChessBoard& board);
    
public:
    ChessGUI(int screenWidth, int screenHeight);
    Point getColumnAndRow(int index);

    // Add a board to the GUI
    void addBoard(ChessBoard* board);
    
    // Input
    void handleKeyboardInputs(ChessBoard& board);
    void handleMouseUpdates(ChessBoard& board);
    

    // Internal Chess API Calls
    bool movePiece();
    void updateAllLegalMoves();

    void runGUI();
};