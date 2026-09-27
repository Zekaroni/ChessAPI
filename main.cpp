#include "./video/ChessGUI.cpp"

int main()
{
    int screenWidth  = 900;
    int screenHeight = 900;
    
    ChessLogic chess;
    ChessGUI chessGUI(&chess, screenWidth, screenHeight);

    int boardSize = chessGUI.getBoardSize();
    int boardX    = (screenWidth - boardSize) / 2;
    int boardY    = (screenWidth - boardSize) / 2;
    chessGUI.setBoardPostion(boardX,boardY);
    
    chess.loadFEN("6k1/p1pqn2p/6p1/2Np1p2/3P1B2/4P3/3QKPPP/1r5R b - - 1 24");

    chessGUI.runGUI();
    return 0;
}