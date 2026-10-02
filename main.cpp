#include "./video/ChessGUI.cpp"
#include "./devtool/Devtool.cpp"
#include <thread>

int main()
{
    if (loggingHelper.logStartupTime()) loggingHelper.startStartupTimer();
    loggingHelper.globalLog(true);
    
    int screenSize = 900;
    int chessGameCount = 10000;

    int chessGridAmount = (int)std::sqrt(chessGameCount);

    int boardSpacing = screenSize / chessGridAmount;
    screenSize = boardSpacing * chessGridAmount;
    chessGameCount = chessGridAmount * chessGridAmount;

    ChessGUI chessGUI(screenSize, screenSize);
    std::vector<ChessLogic> chessGames;
    std::vector<ChessBoard> chessBoards;

    chessGames.reserve(chessGameCount);
    chessBoards.reserve(chessGameCount);
    for (int j = 0; j < chessGridAmount; j++)
    {
        for (int i = 0; i < chessGridAmount; i++)
        {
            chessGames.emplace_back();
            chessBoards.emplace_back(ChessBoard(&chessGames[i], {i * boardSpacing}, (j * boardSpacing), boardSpacing));
            chessGUI.addBoard(&chessBoards.back());
        }
    }
    
    // chessGames[0].loadFEN("1r2qrk1/pN4p1/2b1p2p/3nN3/P2P1p2/Q7/1P3PPP/R2R2K1 b");
    
    
    if (loggingHelper.logStartupTime())
    {
        loggingHelper.endStartupTimer();
        loggingHelper.logStartupTimer(false);
    }
    

    chessGUI.runGUI();

    return 0;
}