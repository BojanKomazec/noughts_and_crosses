
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
    unsigned int score_;

public: 
    Player() :
        name_(""), token_(tictactoe::entry::empty), score_(0) {}

    Player(const std::string& name, tictactoe::entry token) :
        name_(name), token_(token), score_(0) {}

    const std::string& Name() const
    {
        return name_;
    }

    const tictactoe::entry& Token() const
    {
        return token_;
    }

    unsigned int GetScore() const
    {
        return score_;
    }

    void AddWinnerScore()
    {
        score_ += 2;
    }

    void AddDrawScore()
    {
        score_ += 1;
    }
};

void ObtainPlayerName(unsigned short int player_number, std::string& player_name)
{
    std::cout << "Please enter player" << player_number << "'s name: "; 
    std::cin >> player_name;
}

bool PlayerWantsNextRound(const std::string& player_name)
{
    bool continue_game = false;

    while (true)
    {
        std::cout << player_name << ", do you wish to play another game? (y/n)";
    
        std::string answer;
        std::cin >> answer;

        if (answer == "y" || answer == "Y")
        {
            continue_game = true;
            break;
        }
        else if (answer == "n" || answer == "N")
        {
            break;
        }
        else
        {
            std::cout << "\nPlease enter 'y' or 'n'.";
        }
    }

    return continue_game;
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
    size_t starting_player_index = 0;

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
        size_t player_index = starting_player_index;
        size_t cells_populated_count = 0;

        while (true)
        {
            int x, y;

            Player& player = players_.at(player_index);

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
                        ++cells_populated_count;
                        break;
                    }
                    catch (const std::out_of_range& exc)
                    {
                        std::cout << exc.what() << "\n";
                    }
                }
            }

            std::cout << board_;

            if (cells_populated_count >= 5)
            {
                bool isWinningMove = tictactoe::check_winner(board_, player.Token());

                if (isWinningMove)
                {
                    std::cout << "\n" << player.Name() << " has won the game!\n";
                    player.AddWinnerScore();
                    break;
                }
                else
                {
                    if (cells_populated_count == 9)
                    {
                        std::cout << "\n" << "No winner in this game...Draw!\n";

                        // WARNING! use 'auto&'
                        for (auto& player : players_)
                        {
                            player.AddDrawScore();
                        }

                        break;
                    }
                }
            }

            player_index = 1 - player_index;
        }

        starting_player_index = 1 - starting_player_index;
    }

    void PrintCurrentScore()
    {
        std::cout << "\nCurrent score: " <<
            players_[0].Name() << " : " << players_[0].GetScore() << ", " <<
            players_[1].Name() << " : " << players_[1].GetScore() << "\n";
    }

    bool PlayersWantNextRound()
    {
        return PlayerWantsNextRound(players_[0].Name()) && PlayerWantsNextRound(players_[1].Name());
    }

    void ClearBoard()
    {
        clear_board(board_);
    }
};

int main()
{
#ifdef TEST
    test1();
#endif

    Game game;

    while (true)
    {
        game.Play();
        game.PrintCurrentScore();

        if (game.PlayersWantNextRound())
        {
            game.ClearBoard();
        }
        else
        {
            break;
        }
    }

    std::cout << "\nGame over! Press ENTER to quit...";
    std::cin.ignore();
    std::cin.get();
}