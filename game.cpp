
#include "tictactoe.hpp"
#include <cassert>
#include <string>
#include <iostream>


#ifdef TEST

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

#endif

class Player
{
    std::string name_;
    tictactoe::entry token_;

public: 
    Player() :
        name_(""), token_(tictactoe::entry::empty) {}

    Player(const std::string& name, tictactoe::entry token) :
        name_(name), token_(token) {}

    const std::string& Name() const
    {
        return name_;
    }

    const tictactoe::entry& Token() const
    {
        return token_;
    }
};

void ObtainPlayerName(unsigned short int player_number, std::string& player_name)
{
    std::cout << "Please enter player" << player_number << "'s name: "; 
    std::cin >> player_name;
}

tictactoe::entry ObtainPlayerToken(const std::string& player_name)
{
    tictactoe::entry token;
    std::string token_string;
    std::cout << player_name << ", please enter your token choice (X or O): ";
    std::cin >> token_string;

    if (token_string == "X" || token_string == "x")
    {
        token = tictactoe::entry::cross;
    }
    else if (token_string == "O" || token_string == "o")
    {
        token = tictactoe::entry::nought;
    }
    else
    {
        throw std::exception("Invalid token character");
    }

    return token;

}

class Game
{
    std::array<Player, 2> players_;
    tictactoe::board board_;
public:
    Game()
    {
        std::string player_name;
        ObtainPlayerName(1, player_name);
        tictactoe::entry token = ObtainPlayerToken(player_name);
        players_.at(0) = Player(player_name, token);

        ObtainPlayerName(2, player_name);
        if (token == tictactoe::entry::cross)
        {
            token = tictactoe::entry::nought;
        }
        else
        {
            token = tictactoe::entry::cross;
        }

        players_.at(1) = Player(player_name, token);
    }

    void Play()
    {
        size_t index = 0;

        while (true)
        {
            int x, y;

            const Player& player = players_.at(index);

            while (true)
            {
                std::cout << player.Name() << ", enter the x coordinate of the field you want to put your token: ";
                std::cin >> x;
                std::cout << player.Name() << ", enter the y coordinate of the field you want to put your token: ";
                std::cin >> y;

                if (board_(x, y) != tictactoe::entry::empty)
                {
                    std::cout << "That field has already been populated, please choose another one.\n";
                }
                else
                {
                    try
                    {
                        board_(x, y) = player.Token();
                        break;
                    }
                    catch (const std::out_of_range& exc)
                    {
                        std::cout << exc.what() << "\n";
                    }
                }
            }

            std::cout << board_;

            bool isWinningMove = tictactoe::check_winner(board_, player.Token());

            if (isWinningMove)
            {
                std::cout << "\n" << player.Name() << " has won the game!\n";
                break;
            }

            index = 1 - index;
        }
    }
};

int main()
{
#ifdef TEST
    test1();
#endif

    Game game;
    game.Play();

    std::cout << "\nPress ENTER to quit...";
    std::cin.ignore();
    std::cin.get();
}