//
// Created by Tristan Brindle on 14/11/2017.
//

#include "tictactoe.hpp"

namespace tictactoe {

std::ostream& operator<<(std::ostream& os, entry e)
{
    switch (e) {
    case entry::empty:
        return os << " ";
    case entry::nought:
        return os << "O";
    case entry::cross:
        return os << "X";
    }
}

board::board()
{
    array_.fill(entry::empty);
}

entry board::operator()(int row, int col) const
{
    if (row < 0 || row > 2) {
        throw std::out_of_range{
                "Row must be in the range 0-2, got " + std::to_string(row)};
    }
    if (col < 0 || col > 2) {
        throw std::out_of_range{
                "Column must be in the range 0-2, got " + std::to_string(col)};
    }

    return array_[row * 3 + col];
}

entry& board::operator()(int row, int col)
{
    if (row < 0 || row > 2) {
        throw std::out_of_range{
                "Row must be in the range 0-2, got " + std::to_string(row)};
    }
    if (col < 0 || col > 2) {
        throw std::out_of_range{
                "Column must be in the range 0-2, got " + std::to_string(col)};
    }

    return array_[row * 3 + col];
}

std::ostream& operator<<(std::ostream& os, const board& b)
{
    os << "\n";
    os << " " << b(0, 0) << " | " << b(0, 1) << " | " << b(0, 2) << "\n";
    os << "---+---+---\n";
    os << " " << b(1, 0) << " | " << b(1, 1) << " | " << b(1, 2) << "\n";
    os << "---+---+---\n";
    os << " " << b(2, 0) << " | " << b(2, 1) << " | " << b(2, 2) << "\n\n";
    return os;
}

// transforms 2D board into 1D array of bits where each entry e is replaced by 1 and other entries by 0
// e.g.
//    x_o
//    __x
//    o_x
// will be transformed to 100001001
unsigned short int flatten(const board& b, entry e)
{
    unsigned short int flattened_mono_board = 0;
    unsigned short int flag = 0b100000000;

    for(auto it = b.begin(); it != b.end(); ++it)
    {
        if (*it == e)
        {
            flattened_mono_board |= flag;
        }

        flag >>= 1;
    }

    return flattened_mono_board;
}

// TODO: Implement this function correctly
bool check_winner(const board& b, entry e)
{
    // winning token positions
    static unsigned short int masks[8] =
    {
        0b111000000,
        0b000111000,
        0b000000111,
        0b100100100,
        0b010010010,
        0b001001001,
        0b100010001,
        0b001010100
    };

    unsigned short int flattened_mono_board = flatten(b, e);

    for (auto mask : masks)
    {
        if ((unsigned short int)(flattened_mono_board & mask) == mask)
            return true;
    }

    return false;
}

// TODO: Implement this function correctly
bool check_draw(const board&)
{
    return false;
}

void clear_board(board& b)
{
    for (size_t x = 0; x <= 2; ++x)
        for (size_t y = 0; y <= 2; ++y)
            b(x, y) = tictactoe::entry::empty;
}

}
