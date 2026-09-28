#include "./video/ChessGUI.cpp"

int main()
{
    if (loggingHelper.logStartupTime()) loggingHelper.startStartupTimer();

    
    int screenSize  = 920;
    ChessGUI chessGUI(screenSize, screenSize);
    ChessLogic chess;
    ChessBoard board(&chess,0,0,screenSize);
    chessGUI.addBoard(&board);
    chess.loadFEN("1r2qrk1/pN4p1/2b1p2p/3nN3/P2P1p2/Q7/1P3PPP/R2R2K1 b");
    
    // int chessGridAmount = 8;
    // const int chessGameCount = 64;
    // std::vector<ChessLogic> chessGames;
    // std::vector<ChessBoard> chessBoards;

    // chessGames.reserve(chessGameCount);
    // chessBoards.reserve(chessGameCount);
    // for (int j = 0; j < chessGridAmount; j++)
    // {
    //     for (int i = 0; i < chessGridAmount; i++)
    //     {
    //         chessGames.emplace_back();
    //         chessBoards.emplace_back(ChessBoard(&chessGames[i], {i * 100}, (j * 100), 100));
    //         chessGUI.addBoard(&chessBoards.back());
    //     }
    // }
    
    if (loggingHelper.logStartupTime())
    {
        loggingHelper.endStartupTimer();
        loggingHelper.logStartupTimer(false);
    }
    
    chessGUI.runGUI();
    return 0;
}