
#include "tictactoe.hpp"

#include <cassert>

//  xxo
//  oxx
//  oxo

// flattening by x:
// 110
// 011
// 010
// = 110 011 010
//   010 010 010
void test1()
{
    tictactoe::board b;
    b(0, 0) = tictactoe::entry::cross;
    b(0, 1) = tictactoe::entry::cross;
    b(0, 2) = tictactoe::entry::nought;

    b(1, 0) = tictactoe::entry::nought;
    b(1, 1) = tictactoe::entry::cross;
    b(1, 2) = tictactoe::entry::cross;

    b(2, 0) = tictactoe::entry::nought;
    b(2, 1) = tictactoe::entry::cross;
    b(2, 2) = tictactoe::entry::nought;

    assert(tictactoe::flatten(b, tictactoe::entry::cross) == 0b110011010);
    assert(tictactoe::check_winner(b, tictactoe::entry::cross) == true);
}

int main()
{
    test1();
}