#include "Devtool.h"

Devtool::Devtool(ChessGUI* chessGraphics)
: chessGraphics(chessGraphics)
{
    width = 500;
    height = 500;
}

void Devtool::run()
{
   chessGraphics->runGUI();
}