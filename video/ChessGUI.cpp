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
    _boardFontSize  = _cellSize / 4;
    _cursorPosition = 0;
    _internalChessLogic       = chessInstance;
    _currentHighlightBitboard = (bitboard_t)0;
};

void ChessBoard::setBoardSize(int size)
{
    _boardSize = size;
    _cellSize = _boardSize / _cellsPerRow;
    _boardSize = _cellSize * _cellsPerRow;
    _boardFontSize = _boardSize/(_cellsPerRow*6);
}


// Setters

void ChessBoard::setBoardPostion(int x, int y)
{
    _boardX = x;
    _boardY = y;
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

void ChessGUI::initalize()
{
    SetTraceLogLevel(LOG_NONE);
    SetTargetFPS(60);

    InitWindow(_screenWidth, _screenHeight, "Chess");

    // NOTE:
    //     this creates a cache for us to draw to so we dont have to render
    //     the image every frame, but rather only when there is a cahnge
    _boardFrameCache   = LoadRenderTexture(_screenWidth, _screenHeight);

    _biggestDimesion = std::max({_screenWidth, _screenHeight});
    _maxCellSize  = _biggestDimesion / ROW_COUNT;
    _maxBoardSize = _maxCellSize * ROW_COUNT;
    _boardTextureCache = LoadRenderTexture(_maxBoardSize, _maxBoardSize);

    Image windowIcon = LoadImage("./assets/images/icon.png");
    SetWindowIcon(windowIcon);
    UnloadImage(windowIcon);

    cachePieceTextures();
    cacheBoardTexture();
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
void ChessGUI::cachePieceTextures()
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
        _pieceTextures[i+6] = LoadTextureFromImage(img); // black pieces
        ImageColorInvert(&img);  // for white pieces
        _pieceTextures[i] = LoadTextureFromImage(img);
        UnloadImage(img);
    }
}

void ChessGUI::cacheBoardTexture()
{
    BeginTextureMode(_boardTextureCache);
    ClearBackground(CHESS_GLOBALS::COLORS::WHITE_SIDE);
    for(int j = 0; j < ROW_COUNT; j++)
    {
        for (int i = 0; i < COLUMN_COUNT / 2; i++)
        {
            DrawRectangle(
                (i * _maxCellSize * 2) + ((j % 2 == 0) ? _maxCellSize : 0),
                (j * _maxCellSize),
                _maxCellSize,
                _maxCellSize,
                *CHESS_GLOBALS::COLORS::PLAYERS[BLACKSIDE]
            );
        }
    }
    EndTextureMode();
}

void ChessGUI::renderBoardCache()
{
    // TODO: Maybe make this stored in each board and have a setup where only the one board renders
    //       that way if we are simulating hundreds of games and rendering them, they all can draw
    //       independantly.
    _totalFrames++;
    BeginTextureMode(_boardFrameCache);
    ClearBackground(CHESS_GLOBALS::COLORS::BACKGROUND);
    for (ChessBoard* board: _boards)
    {
        renderBoard(*board);
        renderFileRankText(*board);
        highlightCursor(*board);
        hightlightCurrentBitboardCells(*board);
        renderPieces(*board);
    }
    EndTextureMode();
    _hasChange = false;
}


void ChessGUI::renderBoard(ChessBoard& board)
{
    Rectangle source = {
        0.0f, 0.0f,
        (float)_boardTextureCache.texture.width,
        -(float)_boardTextureCache.texture.height
    };
    Rectangle destination = {
        (float)board.boardX(), (float)board.boardY(),
        (float)board.boardSize(),
        (float)board.boardSize()
    };

    DrawTexturePro(
        _boardTextureCache.texture,
        source,
        destination,
        {0.0f,0.0f},
        0.0f,
        WHITE
    );
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
                // NOTE: May be useful to calculate more acurately
                // const char* file = TextFormat("%c", CHESS_GLOBALS::FILES::STRING[i]);
                // int textWidth = MeasureText(file, board.boardFontSize());

                float x = board.boardX() + ((float)i + 0.75) * board.cellSize();
                float y = board.boardY() + ((float)j + 0.75) * board.cellSize();

                DrawText(
                    TextFormat("%c", CHESS_GLOBALS::FILES::STRING[i]),
                    x,
                    y,
                    board.boardFontSize(),
                    *CHESS_GLOBALS::COLORS::PLAYERS[(i % 2)]
                );
            }
        }
    }
}

void ChessGUI::renderPieces(ChessBoard& board)
{
    Point position;
    piece_t currentPiece;
    for (int i = 63; i >= 0; i--)
    {
        currentPiece = board.internalChessLogic()->boardState[63-i];
        if (!currentPiece) continue;
        position = getColumnAndRow(i);
        
        Texture2D pieceTexture = _pieceTextures[board.internalChessLogic()->playerPieceToTextureIndexHash[currentPiece]];
        
        Rectangle source = {
            0.0f,
            0.0f,
            (float)pieceTexture.width,
            (float)pieceTexture.height
        };

        Rectangle destination = {
            (float)(board.boardX() + (position.x) * board.cellSize()),
            (float)(board.boardY() + (position.y) * board.cellSize()),
            (float)(board.cellSize()),
            (float)(board.cellSize())
        };

        DrawTexturePro(
            pieceTexture,
            source,
            destination,
            {0.0f,0.0f},
            0.0f,
            WHITE
        );
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

            if (_hasChange) renderBoardCache();

            BeginDrawing();

            Rectangle source = {
                0.0f, 0.0f,
                (float)_boardFrameCache.texture.width,
                -(float)_boardFrameCache.texture.height
            };
            Rectangle destination = {
                0.0f, 0.0f,
                (float)_screenWidth,
                (float)_screenHeight
            };

            DrawTexturePro(
                _boardFrameCache.texture,
                source,
                destination,
                {0.0f,0.0f},
                0.0f,
                WHITE
            );
    
            EndDrawing();
        }
    }
    CloseWindow();
};