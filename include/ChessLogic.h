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
    pieceBitboard pieceBitboards[2] = {0};
    uint64_t occupiedBoard          =  0 ;

public:
    ChessLogic();
    void printBoard();
    void print_bits(uint64_t value)
    {
        for (int i = 63; i >= 0; i--)
        {
            std::cout << ((value >> i) & 1);
            if ((i) % 8 == 0)
            {
                std::cout << '\n';
            }
        }
    }
};