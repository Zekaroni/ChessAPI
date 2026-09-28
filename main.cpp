#include "./video/ChessGUI.cpp"

int main()
{
    int screenWidth  = 900;
    int screenHeight = 900;
    int boardSize = 500;
    ChessLogic chess;
    ChessGUI chessGUI(&chess, screenWidth, screenHeight);

    // BUG: Changing size and scaling piece images is broken
    // chessGUI.setBoardSize(boardSize);
    boardSize = chessGUI.getBoardSize();
    int boardX    = (screenWidth - boardSize) / 2;
    int boardY    = (screenWidth - boardSize) / 2;
    chessGUI.setBoardPostion(boardX,boardY);
    
    chess.loadFEN("6k1/p1pqn2p/6p1/2Nppp2/3P1B2/4Pp2/3QKPPP/1r5R b");

    chessGUI.runGUI();
    return 0;
}