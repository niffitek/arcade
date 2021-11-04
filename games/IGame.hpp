/*
** EPITECH PROJECT, 2021
** B_OOP_400_BER_4_1_arcade_adrian_lewitzki
** File description:
** IGame.hpp
*/

#ifndef B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_IGAME_HPP
#define B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_IGAME_HPP

#include <string>
#include "../graphical/IGraphics.hpp"

class IGame
{
protected:
    IGraphics *lib;
public:
    virtual void start_game() = 0;
    virtual std::string game_tick() = 0;
    virtual bool is_running() = 0;
    virtual void use_graphics_lib(IGraphics *lib) = 0;
    virtual ~IGame() = default;
    virtual void reload_all_objects() = 0;
};

#endif //B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_IGAME_HPP
