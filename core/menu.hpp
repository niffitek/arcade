/*
** EPITECH PROJECT, 2019
** FILE_NAME
** File description:
** FILE DESCRIPTION
*/

#ifndef B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_MENU_HPP
#define B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_MENU_HPP

#include "../games/IGame.hpp"
#include "graphical.hpp"
#include <chrono>
#include <iostream>
#include <vector>
#include <memory>
typedef struct
{
    int x1;
    int x2;
    int y1;
    int y2;
} pos;

template<typename T1, typename  T2, typename  T3>
class Triple
{
public:
    T1 first;
    T2 second;
    T3 third;
};

class Menu final: IGame
{
private:
    void *_handle_game;
    void *_handle_gr;
    bool _isRunning = false;
    int _selected_y = 0;
    int _selected_x = 0;
    int _selected_game = 0;
    int _selected_gr = 0;
    Triple<object_creation_data, pos, void *>_name;
    std::string _pressed;
    std::vector<Triple<object_creation_data, pos, void *>>_headlines;
    std::vector<Triple<object_creation_data, pos, void *>>_scores;
    std::vector<Triple<object_creation_data, pos, void *>>_names;
    std::vector<Triple<object_creation_data, pos, void *>>_gr_libs;
    std::vector<Triple<object_creation_data, pos, void *>>_game_libs;
    Triple<object_creation_data, pos, void *>_arrow;
    Triple<object_creation_data, pos, void *>_player;
    std::vector<std::string> get_game_libs();
    std::vector<std::string> get_graphical_libs();
    IGame *_game;
    void change_game();
    std::string &moveGame(std::string &ret);
    void change_graphics();
    void draw_all();
    void create_arrow();
    void create_player();
    void move_up_down();
    void insert_graphicallibs(const std::vector<std::string> &graphical_libs);
    void insert_gamelibs(const std::vector<std::string> &game_libs);
    void create_headline(int, int, std::string);
    void InsertGameScores(std::string game_name, int it);

public:
    void start_game() final;
    std::string game_tick() final;
    bool is_running() final;
    void use_graphics_lib(IGraphics *lib) final;
    void change_selected();
    void set_handler(void *hd);
    void reload_all_objects() final;
};

#endif //B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_MENU_HPP
