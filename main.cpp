#include "./video/ChessGUI.cpp"

int main()
{
    // BUG: It's not a bug in the traditional sense but the window takes
    //      a while compared to what it used to when first opening.
    if (loggingHelper.logStartupTime()) loggingHelper.startStartupTimer();

    int screenWidth  = 1200;
    int screenHeight = 900;
    ChessGUI chessGUI();
    int chessGridAmount = 8;

    for (int i = 0; i < chessGridAmount; i++)
    {
        chessGUI;
    }


    if (loggingHelper.logStartupTime())
    {
        loggingHelper.endStartupTimer();
        loggingHelper.logStartupTimer(false);
    }

    return 0;
}