#include "./video/ChessGUI.cpp"

int main()
{
    // BUG: It's not a bug in the traditional sense but the window takes
    //      a while compared to what it used to when first opening.
    if (loggingHelper.logStartupTime()) loggingHelper.startStartupTimer();

    int screenWidth  = 1200;
    int screenHeight = 900;
    int boardSize    = 400;

    ChessLogic chessLogic1;
    ChessLogic chessLogic2;
    ChessLogic chessLogic3;
    ChessLogic chessLogic4;

    chessLogic1.loadFEN("6k1/p1pqn2p/6p1/2Nppp2/3P1B2/4Pp2/3QKPPP/1r5R b");
    chessLogic2.loadFEN("8/1p5R/2k3p1/8/2KP1p2/r4P1P/2n3P1/6R1 b");
    chessLogic3.loadFEN("2k1r3/1pB3pp/pPn5/5bP1/6B1/2P4P/P3pp2/2KR4 w");

    ChessGUI chessGUI(screenWidth, screenHeight);
    
    int board1X = 200;
    int board1Y =  40;
    ChessBoard board1(&chessLogic1, board1X, board1Y, boardSize);

    int board2X = 630;
    int board2Y =  40;
    ChessBoard board2(&chessLogic2, board2X, board2Y, boardSize);
    
    int board3X = 200;
    int board3Y = 470;
    ChessBoard board3(&chessLogic3, board3X, board3Y, boardSize);

    int board4X = 630;
    int board4Y = 470;
    ChessBoard board4(&chessLogic4, board4X, board4Y, boardSize);


    chessGUI.addBoard(board1);
    chessGUI.addBoard(board2);
    chessGUI.addBoard(board3);
    chessGUI.addBoard(board4);


    if (loggingHelper.logStartupTime())
    {
        loggingHelper.endStartupTimer();
        loggingHelper.logStartupTimer(false);
    }

    chessGUI.runGUI();

    return 0;
}