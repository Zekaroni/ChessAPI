#include "ChessGUI.h"

//-----// ChessBoard Class Methods //-----//
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
    _hasUpdate = true;
};


//---// Setters

/// @brief Sets the absolute position relative to the window
/// @param x horizontal position left to right
/// @param y vertical posion top to bottom
void ChessBoard::setBoardPostion(int x, int y)
{
    _boardX = x;
    _boardY = y;
}

void ChessBoard::setBoardSize(int size)
{
    _boardSize = size;
    _cellSize = _boardSize / _cellsPerRow;
    _boardSize = _cellSize * _cellsPerRow;
    _boardFontSize = _boardSize/(_cellsPerRow*6);
}

/// @brief Reloads and rerenders entire board texture
/// @param gui pointer to instance of ChessGUI
void ChessBoard::refreshBoardTexture(ChessGUI* gui)
{
    _initalizeBoardTexture(gui);
    _renderFileRankTextToTexture();
    _highlightCursor(gui);
    _hightlightCurrentBitboardCells(gui);
    _renderPiecesToTexture(gui);
}

Texture2D* ChessBoard::getBoardTexture()
{
    return &_currentBoardTexture.texture;
}

/// @brief Checks if the board has an update to render
/// @return True if the board has an update, false otherwise
bool ChessBoard::hasUpdate()
{
    return _hasUpdate;
}


/// @brief Creates a board texture that will dynamically render it's own game
/// @param gui pointer to instance of ChessGUI
void ChessBoard::_initalizeBoardTexture(ChessGUI* gui)
{
    // NOTE: It may be smart to save the texture as a lot smaller of an texture
    // Copies the board texture as the biggest possible board size
    _currentBoardTexture = LoadRenderTexture(gui->getBoardTexture().width, gui->getBoardTexture().height);
    BeginTextureMode(_currentBoardTexture);
    DrawTexture(gui->getBoardTexture(), 0, 0, WHITE);
    EndTextureMode();
}

/// @brief Renders the `ChessBoard`'s pieces to the screen
/// @param board pointer to instance of ChessGUI
void ChessBoard::_renderPiecesToTexture(ChessGUI* gui)
{
    // NOTE:
    //     Same as the "_initalizeBoardTexture" where we should render these
    //     locally to the texture AND only rerender the piece(s) that
    //     was/were effected
    
    Point position;
    piece_t currentPiece;
    BeginTextureMode(_currentBoardTexture);
    DrawTexture(gui->getBoardTexture(), 0, 0, WHITE);
    for (int i = 63; i >= 0; i--)
    {
        currentPiece = _internalChessLogic->boardState[63-i];
        if (!currentPiece) continue;
        position = gui->getColumnAndRow(i);
        
        Texture2D pieceTexture = gui->getPieceTexture(currentPiece);
        
        Rectangle source = {
            0.0f,
            0.0f,
            (float)pieceTexture.width,
            (float)pieceTexture.height
        };

        Rectangle destination = {
            (float)(position.x * _cellSize),
            (float)(position.y * _cellSize),
            (float)(_cellSize),
            (float)(_cellSize)
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
    EndTextureMode();
}

/// @brief Draw the file and rank text to the internal texture
/// @param gui pointer to instance of ChessGUI
void ChessBoard::_renderFileRankTextToTexture()
{
    BeginTextureMode(_currentBoardTexture);
    for(int j = 0; j < _cellsPerRow; j++)
    {
        DrawText(
            TextFormat("%d", _cellsPerRow-j),
            0,
            j * _cellSize,
            _boardFontSize,
            *CHESS_GLOBALS::COLORS::PLAYERS[!(j % 2)]
        );
        if (j == (_cellsPerRow - 1))
        {
            for (int i = 0; i < _cellsPerRow; i++)
            {
                // NOTE: May be useful to calculate more acurately
                // const char* file = TextFormat("%c", CHESS_GLOBALS::FILES::STRING[i]);
                // int textWidth = MeasureText(file, board.boardFontSize());

                float x = ((float)i + 0.75) * _cellSize;
                float y = ((float)j + 0.75) * _cellSize;

                DrawText(
                    TextFormat("%c", CHESS_GLOBALS::FILES::STRING[i]),
                    x,
                    y,
                    _boardFontSize,
                    *CHESS_GLOBALS::COLORS::PLAYERS[(i % 2)]
                );
            }
        }
    }
    EndTextureMode();
}


//---// Highlight Methods

/// @brief Highlights the currently selected cell in the ChessBoard instance
/// @param gui pointer to instance of ChessGUI
void ChessBoard::_highlightCursor(ChessGUI* gui)
{
    if (_cursorPosition < 64)
    {
        BeginTextureMode(_currentBoardTexture);
        Point cursor = gui->getColumnAndRow(_cursorPosition);
        DrawRectangle(
            (_cellsPerRow - cursor.x - 1) * _cellSize,
            (_cellsPerRow - cursor.y - 1) * _cellSize,
            _cellSize,
            _cellSize,
            CHESS_GLOBALS::COLORS::CURSOR
        );
        // loggingHelper.streamToTerminal(std::to_string(board.cursorPosition()) + " ");
        EndTextureMode();
    }
}

/// @brief Hightlights the currently selected pieces legal moves
/// @param gui pointer to instance of ChessGUI
void ChessBoard::_hightlightCurrentBitboardCells(ChessGUI* gui)
{
    if (_cursorPosition < 64)
    {
        BeginTextureMode(_currentBoardTexture);
        Point currentPosition;
        for (int i = 63; i >= 0; i--)
        {
            if ((_currentHighlightBitboard >> i) & 1)
            {
                currentPosition = gui->getColumnAndRow(i);
                DrawRectangle(
                    (_cellsPerRow - currentPosition.x - 1) * _cellSize,
                    (_cellsPerRow - currentPosition.y - 1) * _cellSize,
                    _cellSize,
                    _cellSize,
                    CHESS_GLOBALS::COLORS::HIGHLIGHT
                );
            }
        }
        EndTextureMode();
    }
}



//-----// ChessGUI Methods //-----//

/// @brief Creates the top level GUI
/// @param screenWidth absolute width of the main window
/// @param screenHeight absolute height of the main window
ChessGUI::ChessGUI(int screenWidth,int screenHeight)
{
    _screenWidth  = screenWidth;
    _screenHeight = screenHeight;
    _hasChange    = true; // to render first frame
    
    __initalize();
};

/// @brief Initalizes the main GUI
/// @warning Only call this once
void ChessGUI::__initalize()
{
    SetTraceLogLevel(LOG_NONE);
    SetTargetFPS(60);

    InitWindow(_screenWidth, _screenHeight, "Chess");

    // NOTE:
    //     this creates a cache for us to draw to so we dont have to render
    //     the image every frame, but rather only when there is a cahnge
    _fullGUITexture   = LoadRenderTexture(_screenWidth, _screenHeight);

    _biggestDimesion = std::max({_screenWidth, _screenHeight});
    _maxCellSize  = _biggestDimesion / ROW_COUNT;
    _maxBoardSize = _maxCellSize * ROW_COUNT;
    _boardTextureCache = LoadRenderTexture(_maxBoardSize, _maxBoardSize);

    Image windowIcon = LoadImage("./assets/images/icon.png");
    SetWindowIcon(windowIcon);
    UnloadImage(windowIcon);

    _cachePieceTextures();
    _cacheBoardTexture();

    for (ChessBoard* board: _boards)
    {
        board->refreshBoardTexture(this);
    }
}

/// @brief Adds a `ChessBoard` to the GUI to be rendered and handled
/// @param board a pointer to a ChessBoard instance
void ChessGUI::addBoard(ChessBoard* board)
{
    _boards.push_back(board);
    board->refreshBoardTexture(this);
}

/// @brief Reloads all `ChessBoard`'s local textures
void ChessGUI::_refreshAllBoardTextures()
{
    for (ChessBoard* board: _boards)
    {
        board->refreshBoardTexture(this);
    }
}

/// @brief Gets the GUI column and row from the `ChessLogic` format of the board
/// @param index position index
/// @return `Point`{ column, row }
Point ChessGUI::getColumnAndRow(int index)
{
    int row    = int(index / ROW_COUNT);
    int column = int(index % ROW_COUNT);
    Point columnAndRow = {column,row};
    return columnAndRow;
}

Texture2D ChessGUI::getPieceTexture(piece_t currentPiece)
{
    return _pieceTextures[_playerPieceToTextureIndexHash[currentPiece]];
}

Texture2D ChessGUI::getBoardTexture()
{
    return _boardTextureCache.texture;
}


//---// Render and Cache Methods

// NOTE: I tried to order them in order of least to greatest "layer"
//       meaning the first one is the first to render and the next
//       will render over it.

/// @brief Caches all of the piece textures to `_pieceTextures` for better performance
void ChessGUI::_cachePieceTextures()
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
        _pieceTextures[i] = LoadTextureFromImage(img); // black pieces
        ImageColorInvert(&img);  // for white pieces
        _pieceTextures[i+6] = LoadTextureFromImage(img);
        UnloadImage(img);
    }
}

/// @brief Caches the board texture to optimize runtime rendering
void ChessGUI::_cacheBoardTexture()
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

/// @brief Bakes the full GUI to a texture to be rendered if there are no updates
void ChessGUI::_bakeFullGUITexture()
{
    // TODO: Maybe make this stored in each board and have a setup where only the one board renders
    //       that way if we are simulating hundreds of games and rendering them, they all can draw
    //       independantly.

    // _totalFrames++; // was for debugging,commenting out for now
    BeginTextureMode(_fullGUITexture);
    ClearBackground(CHESS_GLOBALS::COLORS::BACKGROUND);
    for (ChessBoard* board: _boards)
    {
        if (board->hasUpdate())
        {
            // board->refreshBoardTexture();
            Texture* boardTexture =  board->getBoardTexture();
            Rectangle source = {
                0.0f, 0.0f,
                (float)(*boardTexture).width,
                -(float)(*boardTexture).height
            };
            Rectangle destination = {
                (float)board->boardX(), (float)board->boardY(),
                (float)board->boardSize(),
                (float)board->boardSize()
            };

            DrawTexturePro(
                *boardTexture,
                source,
                destination,
                {0.0f,0.0f},
                0.0f,
                WHITE
            );
        }   
    }
    EndTextureMode();
    _hasChange = false;
}

void ChessGUI::_renderFullGUITexture()
{
    Rectangle source = {
        0.0f, 0.0f,
        (float)_fullGUITexture.texture.width,
        -(float)_fullGUITexture.texture.height
    };
    Rectangle destination = {
        0.0f, 0.0f,
        (float)_screenWidth,
        (float)_screenHeight
    };

    DrawTexturePro(
        _fullGUITexture.texture,
        source,
        destination,
        {0.0f,0.0f},
        0.0f,
        WHITE
    );
}



//---// Input Methods

/// @brief Handles the keyboard inputs and performs related opperations
/// @param board pointer to instance of ChessBoard
void ChessGUI::handleKeyboardInputs()
{
    // int currentKey = GetKeyPressed();    
    // int tempCursorPosition = board.cursorPosition();
    
    // switch(currentKey)
    // {
    //     case CHESS_GLOBALS::CONTROLS::CYCLE_BITBOARD_UP:
    //     tempCursorPosition += 8;
    //     break;
    //     case CHESS_GLOBALS::CONTROLS::CYCLE_BITBOARD_DOWN:
    //         tempCursorPosition -= 8;
    //     break;
    //     case CHESS_GLOBALS::CONTROLS::CYCLE_BITBOARD_RIGHT:
    //     if (board.cursorPosition() % 8 != 0) tempCursorPosition--;
    //     break;
    //     case CHESS_GLOBALS::CONTROLS::CYCLE_BITBOARD_LEFT:
    //     if (board.cursorPosition() % 8 != 7) tempCursorPosition++;
    //     break;
    // }
    // if (
    //     tempCursorPosition >= 0 &&
    //     tempCursorPosition < 64 &&
    //     tempCursorPosition != board.cursorPosition()
    // )
    // {
    //     board.cursorPosition(tempCursorPosition);
    //     piece_t piece = board.internalChessLogic()->boardState[board.cursorPosition()];
    //     board.setCurrentHighlightBitboard(
    //         board.internalChessLogic()->getPiecePositionBitboard(piece,board.cursorPosition())
    //     );
    // }
}

/// @brief Handles the mouse inputs and performs related opperations
/// @param board pointer to instance of ChessBoard
void ChessGUI::handleMouseUpdates()
{
    // NOTE | BUG:
    //     This really should only be called once since there can only be one
    //     board updated at a time. We should have a way to calculate which
    //     board we are currently in, and then only check updates for it.
    //     This would require keeping track of all board positions locally
    //     or itterating trough each one (less performant I think) and
    //     only calling that board to draw to cached texture and then
    //     returning.
    //
    //     Right now this is called a ridiculous amount of times for no
    //     reason and is still following the old structure which is only
    //     good for when there is one board.

    // int screenX = GetMouseX();
    // int screenY = GetMouseY();
    
    // int mouse_boardX = 0;
    // int mouse_boardY = 0;

    // int mouseCursorPosition = 0;
    
    // // moved this outside for better readability
    // bool insideBoard = screenX < board.boardX() + board.boardSize() && screenX >= 0 + board.boardX() &&
    //                    screenY < board.boardY() + board.boardSize() && screenY >= 0 + board.boardY();
    
    // if (!insideBoard)
    // {
    //     if (board.cursorPosition() != 64)
    //     {
    //         board.cursorPosition(64);
    //         _hasChange = true;
    //     }
    //     return;
    // }
    // mouse_boardX = ((screenX-board.boardX()) / board.cellSize()) + 1;
    // mouse_boardY = (screenY-board.boardY()) / board.cellSize();
    // mouseCursorPosition = board.internalChessLogic()->getIndex({mouse_boardX, mouse_boardY});

    // if (board.cursorPosition() == mouseCursorPosition) { return; }

    // board.cursorPosition(mouseCursorPosition);
    // board.setCurrentHighlightBitboard(
    //     board.internalChessLogic()->allLegalMoves[mouseCursorPosition]
    // );
    // _hasChange = true;
}

/// @brief Runs the main GUI
/// @warning This runs an infinte loop (blocking)
void ChessGUI::runGUI()
{
    _hasChange = true;
    while (!WindowShouldClose())
    {
        if (!_boards.empty())
        {
            // handleMouseUpdates();
            _bakeFullGUITexture();
            BeginDrawing();
            _renderFullGUITexture();
            EndDrawing();
        }
        // loggingHelper.streamToTerminal(TextFormat("FPS: %d", GetFPS()));
    }
    CloseWindow();
};