#include "./video/ChessGUI.cpp"
#include "./devtool/Devtool.cpp"
int main()
{
    if (loggingHelper.logStartupTime()) loggingHelper.startStartupTimer();
    loggingHelper.globalLog(true);
    
    int screenSize  = 900;
    ChessGUI chessGUI(screenSize, screenSize);
    
    int chessGameCount = 1;
    int chessGridAmount = std::sqrt(chessGameCount);
    chessGameCount = chessGridAmount * chessGridAmount;
    loggingHelper.printToTerminal(std::to_string(chessGameCount));
    std::vector<ChessLogic> chessGames;
    std::vector<ChessBoard> chessBoards;

    
    int boardSpacing = screenSize / chessGridAmount;
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
    
    chessGames[0].loadFEN("8/8/8/8/3R4/8/8/8 b");
    
    
    if (loggingHelper.logStartupTime())
    {
        loggingHelper.endStartupTimer();
        loggingHelper.logStartupTimer(false);
    }
    
    chessGUI.runGUI();
    return 0;
}