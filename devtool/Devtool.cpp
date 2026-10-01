#include "Devtool.h"

Devtool::Devtool(ChessGUI* chessGraphics)
 :chessGraphics(chessGraphics)
{
    width = 500;
    height = 500;
}

void Devtool::devtoolGUI()
{
   DrawRectangle(0,0,GetScreenWidth()-width,GetScreenHeight(),{255,255,255,255});
}