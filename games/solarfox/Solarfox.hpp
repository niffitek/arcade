/*
** EPITECH PROJECT, 2019
** FILE_NAME
** File description:
** FILE DESCRIPTION
*/

#ifndef B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_SOLARFOX_HPP
#define B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_SOLARFOX_HPP

#include <vector>
#include "../IGame.hpp"
#include "../../core/graphical.hpp"
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

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

template<typename T1, typename  T2, typename T3>
class Triple
{
public:
    T1 first;
    T2 second;
    T3 third;
};
using object_triple = Triple<object_creation_data, pos, void *>;

class Solarfox final : public IGame
{
private:
    IGraphics *lib = nullptr;
    bool _isRunning;
    std::chrono::steady_clock::time_point _start_tick;
    std::chrono::steady_clock::time_point _last_shot;
    std::chrono::steady_clock::time_point _last_tick;
    pos border = {100, 900, 100, 800};
    std::map<std::string, std::vector<object_triple>> _objects;
    std::pair<object_triple, object_triple> _game_over_text;
    int _cur_direction = 0;
    int _next_direction = 0;
    std::pair<int, int> _move_vec;
    std::pair<object_creation_data, void *> _name;
    std::vector<std::pair<std::string, int>> _scores;
    std::string _input;
    object_triple _player;
    std::vector<int> _move_enemy;
    std::vector<std::pair<int, int>> _move_enemy_projectile;
    std::vector<Triple<int, int, int>> _move_my_projectile;
    std::vector<std::chrono::steady_clock::time_point> _last_enemy_shot;
    bool _created_clocks;
    Triple<object_creation_data, void *, int> _score_data;

    static object_creation_data fill_data(std::string text, object_type type,
                                   std::string path, std::string color);
    static object_triple create_triple(object_creation_data data, pos
    position, void *object);
    void create_game_over();
    void game_over();
    void restart_game();
    void create_borders();
    void create_grid();

    void create_player();
    void get_move();
    void move_player();
    void check_area();

    void create_enemies();
    void move_enemies();
    void create_enemy_projectile(unsigned int enemy);
    void move_enemy_projectile();
    void create_enemy_clocks();
    void check_projectile_col();

    void create_powerups();
    void create_score_data();
    void check_powerups();

    void shot();
    void move_my_projectile();
    void safe_score();
public:
    explicit Solarfox(IGraphics *lib);
    ~Solarfox() final;
    void start_game() final;
    std::string game_tick() final;
    bool is_running() final;
    void use_graphics_lib(IGraphics *lib) final;
    void reload_all_objects() final;
};

extern "C" {
IGame *create_game(IGraphics *lib);
}

#endif //B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_SOLARFOX_HPP
