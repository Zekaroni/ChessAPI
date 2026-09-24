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


class ChessLogic
{
private:
    pieceBitboard pieceBitboards[2];

public:
    void printHello();
};