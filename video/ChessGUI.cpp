#include "ChessGUI.h"

// ChessBoard Class Methods

ChessBoard::ChessBoard(ChessLogic* chessInstance, int boardX, int boardY, int boardSize)
{
    _boardSize      = boardSize;
    _boardX         = boardX;
    _boardY         = boardY;
    _cellsPerRow    = 8;
    _cellSize       = boardSize / 8;
    _boardSize      = _cellSize * 8;
    _boardFontSize  = boardSize/(_cellSize * 3);
    _cursorPosition = 0;
    _internalChessLogic       = chessInstance;
    _currentHighlightBitboard = (bitboard_t)0;

    initPieceTextures();
};

void ChessBoard::setBoardSize(int size)
{
    _boardSize = size;
    _cellSize = _boardSize / _cellsPerRow;
    _boardSize = _cellSize * _cellsPerRow;
    _boardFontSize = _boardSize/(_cellsPerRow*6);
    initPieceTextures();
}


// Setters

void ChessBoard::setBoardPostion(int x, int y)
{
    _boardX = x;
    _boardY = y;
}

void ChessBoard::initPieceTextures()
{
    Image img;
    std::string pathString;
    bool unloadTextures = _pieceTextures[0].id > 0;
    
    for (int i = 0; i < PIECE_TEXTURE_COUNT/2;i++)
    {
        if (unloadTextures)
        {
            UnloadTexture(_pieceTextures[i]);
            UnloadTexture(_pieceTextures[i+6]);
            _pieceTextures[i] = {};
            _pieceTextures[i+6] = {};
        }
        pathString = std::string("./assets/images/") + CHESS_GLOBALS::INDEX_TO_FEN_LETTER[i+1] + ".png";
        img = LoadImage(pathString.c_str());
        ImageResize  (&img,_cellSize,_cellSize);          // scale to board
        _pieceTextures[i+6] = LoadTextureFromImage(img); // black pieces
        
        ImageColorInvert(&img);  // for white pieces
        _pieceTextures[i] = LoadTextureFromImage(img);
        
        UnloadImage(img);
    }
}


// ChessGUI Methods

ChessGUI::ChessGUI(int screenWidth,int screenHeight)
{
    _screenWidth  = screenWidth;
    _screenHeight = screenHeight;
    _hasChange    = true; // to render first frame
    
    initalize();
};

void ChessGUI::addBoard(ChessBoard& board)
{
    _boards.push_back(&board);
}

Point ChessGUI::getColumnAndRow(int index)
{
    int row    = int(index / ROW_COUNT);
    int column = int(index % ROW_COUNT);
    Point columnAndRow = {column,row};
    return columnAndRow;
}

// Render Methods
// NOTE: I tried to order them in order of least to greatest "layer"
//       meaning the first one is the first to render and the next
//       will render over it.

void ChessGUI::renderBoard(ChessBoard& board)
{
    DrawRectangle(board.boardX(), board.boardY(), board.boardSize(), board.boardSize(), *CHESS_GLOBALS::COLORS::PLAYERS[WHITESIDE]);
    for(int j = 0; j < board.cellsPerRow(); j++)
    {
        for (int i = 0; i < board.cellsPerRow() / 2; i++)
        {
            DrawRectangle(
                board.boardX() + (i * board.cellSize() * 2) + ((j % 2 == 0) ? board.cellSize() : 0),
                board.boardY() + (j * board.cellSize()),
                board.cellSize(),
                board.cellSize(),
                *CHESS_GLOBALS::COLORS::PLAYERS[BLACKSIDE]
            );
        }
    }
};

void ChessGUI::renderFileRankText(ChessBoard& board)
{
    for(int j = 0; j < board.cellsPerRow(); j++)
    {
        DrawText(
            TextFormat("%d", board.cellsPerRow()-j),
            board.boardX(),
            board.boardY() + (j * board.cellSize()),
            board.boardFontSize(),
            *CHESS_GLOBALS::COLORS::PLAYERS[!(j % 2)]
        );
        if (j == (board.cellsPerRow() - 1))
        {
            for (int i = 0; i < board.cellsPerRow(); i++)
            {
                DrawText(
                    TextFormat("%c", CHESS_GLOBALS::FILES::STRING[i]),
                    board.boardX() + (i * board.cellSize()) + (board.cellSize()) - (board.boardFontSize()),
                    board.boardY() + (j * board.cellSize()) + (board.cellSize()) - (board.boardFontSize()),
                    board.boardFontSize(),
                    *CHESS_GLOBALS::COLORS::PLAYERS[(i % 2)]
                );
            }
        }
    }
}

void ChessGUI::renderPieces(ChessBoard& board)
{
    int pieceIndex;
    Point position;
    piece_t currentPiece;
    for (int i = 63; i >= 0; i--)
    {
        currentPiece = board.internalChessLogic()->boardState[63-i];
        if (currentPiece != 0)
        {
            position = getColumnAndRow(i);
            
            DrawTexture(
                board.pieceTextures()[board.internalChessLogic()->playerPieceToTextureIndexHash[currentPiece]],
                board.boardX() + (board.cellSize() * position.x),
                board.boardY() + (board.cellSize() * position.y),
                WHITE
            );
        }
    }
}

void ChessGUI::highlightCursor(ChessBoard& board)
{
    if (board.cursorPosition() < 64)
    {
        Point cursor = getColumnAndRow(board.cursorPosition());
        DrawRectangle(
            board.boardX() + ((board.cellsPerRow() - cursor.x - 1) * board.cellSize()),
            board.boardY() + ((board.cellsPerRow() - cursor.y - 1) * board.cellSize()),
            board.cellSize(),
            board.cellSize(),
            CHESS_GLOBALS::COLORS::CURSOR
        );
        // loggingHelper.streamToTerminal(std::to_string(board.cursorPosition()) + " ");
    }
}

void ChessGUI::hightlightCurrentBitboardCells(ChessBoard& board)
{
    if (board.cursorPosition() < 64)
    {
        Point currentPosition;
        for (int i = 63; i >= 0; i--)
        {
            if ((board.currentHighlightBitboard() >> i) & 1)
            {
                currentPosition = getColumnAndRow(i);
                DrawRectangle(
                    board.boardX() + ((board.cellsPerRow() - currentPosition.x - 1) * board.cellSize()),
                    board.boardY() + ((board.cellsPerRow() - currentPosition.y - 1) * board.cellSize()),
                    board.cellSize(),
                    board.cellSize(),
                    CHESS_GLOBALS::COLORS::HIGHLIGHT
                );
            }
        }
    }
}

void ChessGUI::initalize()
{
    SetTargetFPS(30);
    SetTraceLogLevel(LOG_NONE);
    InitWindow(_screenWidth, _screenHeight, "Chess");
    Image windowIcon = LoadImage("./assets/images/icon.png");
    SetWindowIcon(windowIcon);
    UnloadImage(windowIcon);
}

void ChessGUI::handleInputs(ChessBoard& board)
{
    int currentKey = GetKeyPressed();    
    int tempCursorPosition = board.cursorPosition();
    
    switch(currentKey)
    {
        case CHESS_GLOBALS::CONTROLS::CYCLE_BITBOARD_UP:
        tempCursorPosition += 8;
        break;
        case CHESS_GLOBALS::CONTROLS::CYCLE_BITBOARD_DOWN:
            tempCursorPosition -= 8;
        break;
        case CHESS_GLOBALS::CONTROLS::CYCLE_BITBOARD_RIGHT:
        if (board.cursorPosition() % 8 != 0) tempCursorPosition--;
        break;
        case CHESS_GLOBALS::CONTROLS::CYCLE_BITBOARD_LEFT:
        if (board.cursorPosition() % 8 != 7) tempCursorPosition++;
        break;
    }
    if (
        tempCursorPosition >= 0 &&
        tempCursorPosition < 64 &&
        tempCursorPosition != board.cursorPosition()
    )
    {
        board.cursorPosition(tempCursorPosition);
        piece_t piece = board.internalChessLogic()->boardState[board.cursorPosition()];
        // std::cout << (int)piece << std::endl;
        board.setCurrentHighlightBitboard(
            board.internalChessLogic()->getPiecePositionBitboard(piece,board.cursorPosition())
        );
    }
}
   
void ChessGUI::handleMouse(ChessBoard& board)
{
    int screenX = GetMouseX();
    int screenY = GetMouseY();
    
    int mouse_boardX = 0;
    int mouse_boardY = 0;

    int mouseCursorPosition = 0;
    
    // moved this outside for better readability
    bool insideBoard = screenX < board.boardX() + board.boardSize() && screenX >= 0 + board.boardX() &&
                       screenY < board.boardY() + board.boardSize() && screenY >= 0 + board.boardY();
    
    if (!insideBoard)
    {
        if (board.cursorPosition() != 64)
        {
            board.cursorPosition(64);
            _hasChange = true;
        }
        return;
    }
    mouse_boardX = ((screenX-board.boardX()) / board.cellSize()) + 1;
    mouse_boardY = (screenY-board.boardY()) / board.cellSize();
    mouseCursorPosition = board.internalChessLogic()->getIndex({mouse_boardX, mouse_boardY});

    if (board.cursorPosition() == mouseCursorPosition) { return; }

    board.cursorPosition(mouseCursorPosition);
    piece_t piece = board.internalChessLogic()->boardState[board.cursorPosition()];
    board.setCurrentHighlightBitboard(
        board.internalChessLogic()->getLegalMovesBitboard(piece,board.cursorPosition())
    );
    _hasChange = true;
}

void ChessGUI::runGUI()
{
    while (!WindowShouldClose())
    {
        
        if (!_boards.empty())
        {
            for (ChessBoard* board: _boards)
            {
                handleMouse(*board);
            }
            BeginDrawing();
            if (_hasChange)
            {
                ClearBackground(CHESS_GLOBALS::COLORS::BACKGROUND);
            
                for (ChessBoard* board: _boards)
                {
                    loggingHelper.printToTerminal("Updating GUI\n");
                    renderBoard(*board);
                    renderFileRankText(*board);
                    highlightCursor(*board);
                    hightlightCurrentBitboardCells(*board);
                    renderPieces(*board);
                }
                _hasChange = false;
            }
        }
        EndDrawing();
    }
    CloseWindow();
};