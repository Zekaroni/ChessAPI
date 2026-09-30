#include "../include/utils.h"
#include "../chess/ChessLogic.cpp"
// NOTE: Everthing is top-left oriented for the GUI

class ChessGUI;

class ChessBoard
{
private:
    int _boardSize;
    int _boardX;
    int _boardY;
    int _cellsPerRow;
    int _cellSize;
    int _boardFontSize;
    int _cursorPosition;
    ChessLogic* _internalChessLogic = nullptr;
    bitboard_t  _currentHighlightBitboard;
    RenderTexture2D _currentBoardTexture;

    //---// Render Methods
    void _renderBoard();
    void _renderPiecesToTexture(ChessGUI* gui);

    //---// Highlight and Overlays
    void _highlightCursor(ChessGUI* gui);
    void _hightlightCurrentBitboardCells(ChessGUI* gui);
    void _renderFileRankTextToTexture();

public:
    ChessBoard(ChessLogic* chessInstance, int boardX, int boardY, int boardSize);

    //---// Setters
    void setBoardSize(int size);
    void setBoardPostion(int x, int y);
    void boardFontSize(int fontSize) { _boardFontSize  = fontSize; };
    void cursorPosition(int position){ _cursorPosition = position; };
    

    //---// Getters
    int boardSize()      const { return _boardSize; };
    int boardX()         const { return _boardX; };
    int boardY()         const { return _boardY; };
    int cellsPerRow()    const { return _cellsPerRow; };
    int cellSize()       const { return _cellSize; };
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
    static constexpr uint8_t _playerPieceToTextureIndexHash [16] = {
        255, 0, 1, 2, 3, 4, 5, 255, 255, 6, 7, 8, 9, 10, 11, 255
    };

    //---// Internal Properties Variables
    int _screenWidth;
    int _screenHeight;
    int _biggestDimesion;
    int _maxBoardSize;
    int _maxCellSize;


    std::vector<ChessBoard*> _boards;
    bool _hasChange;

    //---// Textures
    RenderTexture2D _fullGUITexture;
    RenderTexture2D _boardTextureCache;
    Texture2D   _pieceTextures[PIECE_TEXTURE_COUNT] = {};
    
    //---// Debug variables
    uint64_t _totalFrames = 0; // for debug purposes

    
    ///---// Internal Methods
    void __initalize();

    //---// Texture caching
    void _cacheBoardTexture();
    void _cachePieceTextures();
    void _bakeFullGUITexture();
    
    //---// Render Methods
    void renderFullGUITexture();

    
public:
    ChessGUI(int screenWidth, int screenHeight);
    
    //---// Helper Functions
    Point getColumnAndRow(int index);
    void addBoard(ChessBoard* board);
    Texture2D getPieceTexture(piece_t currentPiece);
    
    //---// Input
    void handleKeyboardInputs();
    void handleMouseUpdates();
    

    //---// Internal Chess API Calls
    bool movePiece();
    void updateAllLegalMoves();

    void runGUI();
};