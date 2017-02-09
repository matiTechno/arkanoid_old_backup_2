#include <iostream>
#include <game.hh>

int main()
{
    try
    {
        Game game(500, 500, "game2d");
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    std::cout << '_' << std::endl;
    return 0;
}
