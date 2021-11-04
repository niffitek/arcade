/*
** EPITECH PROJECT, 2021
** B_OOP_400_BER_4_1_arcade_adrian_lewitzki
** File description:
** Nibbler.hpp
*/

#ifndef B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_NIBBLER_HPP
#define B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_NIBBLER_HPP

#include <vector>
#include "../IGame.hpp"
#include "../../core/graphical.hpp"
#include <chrono>
#include <iostream>
#include <memory>

typedef struct
{
    int x1;
    int x2;
    int y1;
    int y2;
} pos;

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

template<typename T1, typename  T2, typename  T3>
class Triple
{
public:
    T1 first;
    T2 second;
    T3 third;
};

class Nibbler final : public IGame
{
private:
    int _fast = 0;
    Triple<object_creation_data, pos, void *> boarder;
    int score = 0;
    std::pair<object_creation_data, void *> score_data;
    std::string _direction;
    std::chrono::steady_clock::time_point _last_tick;
    std::chrono::steady_clock::time_point _last_fruit;
    bool _isRunning;
    std::vector<Triple<object_creation_data, pos, void *>> _snake;
    int _cur_direction;
    std::pair<int, int> _mov;
    Triple<object_creation_data, pos, void *> _fruit;
    std::pair<object_creation_data, void *> _name;
    std::vector<std::pair<std::string, int>> _scores;
    Triple<object_creation_data, pos, void *> _gameover_screen1;
    Triple<object_creation_data, pos, void *> _gameover_screen2;
    std::vector<Triple<object_creation_data, pos, void *>> shits;
    std::vector<pos> _last_pos;
    bool _shit = false;

    void change_move();
    void create_fruit();
    bool check_collision();
    void make_longer(int x = 0, int y = 0);
    void create_start_snake();
    void move_snake();
    void check_fruit();
    void print_gameover();
    void draw_boarders();
    void restart_game();
    void col_end();

public:
    ~Nibbler();
    explicit Nibbler(IGraphics *lib);
    void start_game() final;
    std::string game_tick() final;
    bool is_running() final;
    void use_graphics_lib(IGraphics *lib) final;
    void reload_all_objects() final;
};

extern "C" {
IGame *create_game(IGraphics *lib);
}

#endif //B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_NIBBLER_HPP
