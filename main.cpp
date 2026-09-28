#include "./video/ChessGUI.cpp"

int main()
{
    int screenWidth  = 900;
    int screenHeight = 900;
    int boardSize    = 300;

    ChessLogic chessLogic1;
    ChessLogic chessLogic2;

    chessLogic1.loadFEN("6k1/p1pqn2p/6p1/2Nppp2/3P1B2/4Pp2/3QKPPP/1r5R b");

    ChessGUI chessGUI(screenWidth, screenHeight);
    
    int board1X = 0;
    int board1Y = 0;
    ChessBoard board1(&chessLogic1, board1X, board1Y, boardSize);

    int board2X = 400;
    int board2Y =   0;
    ChessBoard board2(&chessLogic2, board2X, board2Y, boardSize);


    chessGUI.addBoard(board1);
    chessGUI.addBoard(board2);

    chessGUI.runGUI();

    return 0;
}