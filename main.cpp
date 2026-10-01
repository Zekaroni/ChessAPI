#include "./video/ChessGUI.cpp"
#include "./devtool/Devtool.cpp"
int main()
{
    if (loggingHelper.logStartupTime()) loggingHelper.startStartupTimer();

    
    int screenSize  = 900;
    ChessGUI chessGUI(screenSize, screenSize);
    ChessLogic chess;
    ChessBoard board(&chess,0,0,screenSize);
    chessGUI.addBoard(&board);
    chess.loadFEN("8/8/8/8/3R4/8/8/8 b");
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