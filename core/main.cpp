/*
** EPITECH PROJECT, 2019
** FILE_NAME
** File description:
** FILE DESCRIPTION
*/

#include "core.hpp"
#include "graphical.hpp"
#include "menu.hpp"

void display_help() {
    std::cout << "\nUsage:" << std::endl;
    std::cout << "Pass library as first argument.\n" << std::endl;
    std::cout << "Menu:" << std::endl;
    std::cout << "- use 'tab' to switch between:" << std::endl;
    std::cout << "1.) Name editing: Simply type in your username (use 'backspace to delete characters)" << std::endl;
    std::cout << "2.) Choosing graphical library: Simply select the library of your wish with the 'arrow keys' and confirm with 'enter'('return')" << std::endl;
    std::cout << "3.) Choosing the game: Simply select the game of your wish with the 'arrow keys' and confirm with 'enter'('return')\n" << std::endl;
    std::cout << "In-Game options:" << std::endl;
    std::cout << "- use 'M' to get back to menu" << std::endl;
    std::cout << "- use 'O' to switch to previous library and 'P' to switch to next library" << std::endl;
    std::cout << "- use 'K' to switch to previous game and 'L' to switch to next game\n" << std::endl;
    std::cout << "Controls Nibbler:" << std::endl;
    std::cout << "- use 'left arrow' and 'right arrow' to change the direction of the snake" << std::endl;
    std::cout << "- use 'space' to increase the speed of the snake\n" <<
    std::endl;
    std::cout << "Controls Solar Fox:" << std::endl;
    std::cout << "- use 'arrow keys' to change the direction of the player" << std::endl;
    std::cout << "- use 'space' to shoot in the direction of the player" << std::endl;
    std::cout << "- use 'A' to increase the speed of the player" << std::endl;

    exit(0);
}

void print_usage()
{
    std::cout << "Pass library as first argument." << std::endl;
    exit(84);
}

int main(int ac, char **av)
{
    if (ac != 2)
        print_usage();
    if (std::string(av[1]) == "-h" || std::string(av[1]) == "--help")
        display_help();
    Menu m;
    void *lib = dlopen(av[1], RTLD_LAZY);
    if (lib == nullptr)
    {
        std::cout << dlerror() << std::endl;
        return 84;
    }
    auto g = (IGraphics *(*)()) dlsym(lib, "create_graphics");
    if (g == nullptr)
    {
        std::cout << "lib: '" << av[1] << "' is no valid graphical lib" << std::endl;
        return 84;
    }
    IGraphics *glib = g();
    m.use_graphics_lib(glib);
    m.set_handler(lib);
    m.start_game();
    while (m.game_tick().find("exit") == std::string::npos);
    return (0);
}