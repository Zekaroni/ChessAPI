#include "Devtool.h"

int main()
{
    int width  = 800;
    int height = 800;
    ChessLogic debugGame{};
    ChessBoard debugBoard = ChessBoard(&debugGame, 0, 0, 500);
    ChessGUI debugChessGUI{width, height};
    debugChessGUI.addBoard(&debugBoard);
    Devtool devWindow(&debugChessGUI);

    devWindow.run();

    return 0;
}