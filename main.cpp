#include "./chess/ChessLogic.cpp"
#include "./video/ChessGUI.cpp"

int main()
{
    // ChessLogic chessLogic;
    // chessLogic.printBoard();
    int screenWidth  = 900;
    int screenHeight = 900;
    ChessGUI chessGUI(screenWidth, screenHeight);
    int boardSize = chessGUI.getBoardSize();
    int boardX    = (screenWidth - boardSize) / 2;
    int boardY    = (screenWidth - boardSize) / 2;
    chessGUI.setBoardPostion(boardX,boardY);
    chessGUI.runGUI();
    return 0;
}
