#include <cstdint>

#define WHITE 0
#define BLACK 1

struct pieceBitboard
{
    uint64_t pawnBitboard;
    uint64_t knightBitboard;
    uint64_t bishopBitboard;
    uint64_t rookBitboard;
    uint64_t queenBitboard;
    uint64_t kingBitboard;
};

struct Point
{
    int x;
    int y;
};

class ChessLogic
{
private:
    pieceBitboard pieceBitboards[2] = {0};
    uint64_t occupiedBoard          =  0 ;
    uint64_t knightMoveTable[63] = {0};

public:
    ChessLogic(); // Constructor
    void calculateKnightMoves();//Calculates every single possible knight move ~512 bytes
    void printBoard(); // Prints a BitBoard
    void print_bits(uint64_t value);
};