#include <cstdint>

#define WHITE 0
#define BLACK 1

struct pieceBitboard
{
    uint64_t pawnBitborad;
    uint64_t knightBitborad;
    uint64_t bishopBitborad;
    uint64_t rookBitborad;
    uint64_t queenBitborad;
    uint64_t kingBitborad;
};


class ChessLogic
{
private:
    pieceBitboard pieceBitboards[2];

public:
    void printHello();
};