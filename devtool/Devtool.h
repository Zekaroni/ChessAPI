#pragma once    
#include "../video/ChessGUI.h"
class Devtool
{
private:
ChessGUI* chessGraphics;
int width;
int height;
public:
Devtool(ChessGUI* chessGraphics);
void devtoolGUI();
};