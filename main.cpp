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
    
    chessGUI.runGUI();
    return 0;
}
