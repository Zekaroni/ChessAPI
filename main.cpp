#include "./src/ChessLogic.cpp"

int main()
{
    ChessLogic chessLogic;
    chessLogic.calculateKnightMoves();
    chessLogic.printBoard();
    return 0;
}