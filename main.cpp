#include "./video/ChessGUI.cpp"
#include "./devtool/Devtool.cpp"
#include <thread>

int main()
{
    if (loggingHelper.logStartupTime()) loggingHelper.startStartupTimer();
    loggingHelper.globalLog(true);
    
    int screenSize = 1000;
    int chessGameCount = 100'000;

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
            chessGames.push_back(*(new ChessLogic()));
            chessBoards.emplace_back(*(new ChessBoard(&chessGames[i+(chessGridAmount*j)], {i * boardSpacing}, (j * boardSpacing), boardSpacing)));
            chessGUI.addBoard(&chessBoards[i+(chessGridAmount*j)]);
        }
    }
    
    chessGames[0].loadFEN("8/8/8/8/3R4/8/8/8 b");
    
    
    if (loggingHelper.logStartupTime())
    {
        loggingHelper.endStartupTimer();
        loggingHelper.logStartupTimer(false);
    }
    

    chessGUI.runGUI();

    return 0;
}