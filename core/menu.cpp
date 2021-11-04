/*
** EPITECH PROJECT, 2019
** FILE_NAME
** File description:
** FILE DESCRIPTION
*/

#include <dirent.h>
#include "menu.hpp"
#include <algorithm>
#include <utility>
#include <fstream>
#include <sstream>
#include <ncurses.h>

std::vector<std::string> Menu::get_game_libs()
{
    DIR *dir;
    struct dirent *ent;
    std::vector<std::string> libs;
    std::string tmp;
    if ((dir = opendir("./lib/")) == nullptr)
        exit (84);
    while ((ent = readdir(dir)) != NULL) {
        std::string str(ent->d_name);
        if (str.find("arcade_") == 0 && str.find(".so") == str.size() - 3)
        {
            tmp = "./lib/" + str;
            void *_lib = dlopen(tmp.c_str(), RTLD_LAZY);
            if (_lib == nullptr)
                continue;
            auto g = (IGame *(*)()) dlsym(_lib, "create_game");
            if (g != nullptr)
                libs.emplace_back(str.substr(7, str.size() - 10));
        }
    }
    closedir(dir);
    return libs;
}

std::vector<std::string> Menu::get_graphical_libs()
{
    DIR *dir;
    struct dirent *ent;
    std::vector<std::string> libs;
    std::string tmp;
    if ((dir = opendir("./lib/")) == nullptr)
        exit (84);
    while ((ent = readdir(dir)) != NULL) {
        std::string str(ent->d_name);
        if (str.find("arcade_") == 0 && str.find(".so") == str.size() - 3)
        {
            tmp = "./lib/" + str;
            void *_lib = dlopen(tmp.c_str(), RTLD_LAZY);
            if (_lib == nullptr)
                continue;
            auto g = (IGraphics *(*)()) dlsym(_lib, "create_graphics");
            if (g != nullptr)
                libs.emplace_back(str.substr(7, str.size() - 10));
        }
    }
    closedir(dir);
    return libs;
}

void Menu::start_game()
{
    _isRunning = true;
    _selected_y = 0;
    lib->init_screen(1920, 1080);
    _game = nullptr;
    auto game_libs = get_game_libs();
    auto graphical_libs = get_graphical_libs();
    insert_gamelibs(game_libs);
    insert_graphicallibs(graphical_libs);
    create_arrow();
    create_player();
    _selected_y = 0;
    _selected_x = 0;
    _name.first.color_name = "red";
    _name.first.path_to_resource = "assets/font.ttf";
    _name.first.text = "nameless";
    _name.first.type = TEXT;
    _name.third = lib->createObject(&_name.first);
}

void Menu::create_player()
{
    _player.first.text = "name:";
    _player.first.color_name = "green";
    _player.first.path_to_resource = "assets/font.ttf";
    _player.first.type = TEXT;
    _player.second = {550, 550, 40, 40};
    _player.third = lib->createObject(&_player.first);
}

void Menu::create_arrow()
{
    object_creation_data arrow;
    arrow.text = "->";
    arrow.color_name = "green";
    arrow.type = TEXT;
    arrow.path_to_resource = "assets/font.ttf";
    _arrow.first = arrow;
    _arrow.second = _player.second;
    _arrow.second.x1 -= 50;
    _arrow.second.x2 -= 50;
    _arrow.third = lib->createObject(&_arrow.first);
}

void Menu::insert_graphicallibs(const std::vector<std::string> &graphical_libs)
{
    create_headline(550, 100, "graphical:");
    int count = 0;
    for (auto &glib : graphical_libs)
    {
        object_creation_data tmp;
        tmp.text = glib;
        tmp.color_name = "red";
        tmp.type = TEXT;
        tmp.path_to_resource = "assets/font.ttf";
        Triple<object_creation_data, pos, void *> tripleTmp;
        tripleTmp.first = tmp;
        tripleTmp.second = {(150 / (int)graphical_libs.size() * count + 5) * 10,
                          (150 / (int)graphical_libs.size() * count + 5) * 10, 160, 160};
        tripleTmp.third = lib->createObject(&tripleTmp.first);
        _gr_libs.emplace_back(tripleTmp);
        count++;
    }
}

void Menu::create_headline(int x, int y, std::string text)
{
    object_creation_data tmp1;
    tmp1.text = std::move(text);
    tmp1.color_name = "green";
    tmp1.type = TEXT;
    tmp1.path_to_resource = "assets/font.ttf";
    Triple<object_creation_data, pos, void *> tripleTmp;
    tripleTmp.first = tmp1;
    tripleTmp.second = {x, x, y, y};
    tripleTmp.third = lib->createObject(&tripleTmp.first);
    _headlines.emplace_back(tripleTmp);
}

void Menu::insert_gamelibs(const std::vector<std::string> &game_libs)
{
    create_headline(550, 240, "GAMES:");
    int count = 0;
    for (auto &glib : game_libs)
    {
        create_headline((15 + 60 * count) * 10, 320, "NAME:");
        create_headline((40 + 60 * count) * 10, 320, "SCORE:");
        object_creation_data tmp;
        tmp.text = glib;
        tmp.color_name = "red";
        tmp.type = TEXT;
        tmp.path_to_resource = "assets/font.ttf";
        Triple<object_creation_data, pos, void *> tripleTmp;
        tripleTmp.first = tmp;
        tripleTmp.second = {(15 + 60 * count) * 10, (15 + 60 * count) * 10, 280, 280};
        tripleTmp.third = lib->createObject(&tripleTmp.first);
        _game_libs.emplace_back(tripleTmp);
        InsertGameScores(glib, count);
        count++;
    }
}

std::string Menu::game_tick()
{
    std::string ret;
    _pressed.clear();
    if (_game != nullptr) {
        ret = moveGame(ret);
        if (ret.find("next_gr") != std::string::npos)
        {
            _selected_y = _selected_gr + 1;
             if (_selected_y > (int) _gr_libs.size() - 1)
                _selected_y = 0;
             change_graphics();
        }
        if (ret.find("prev_gr") != std::string::npos)
        {
            _selected_y = _selected_gr - 1;
            if (_selected_y < 0)
                _selected_y = (int) _gr_libs.size() - 1;
            change_graphics();
        }
        if (ret.find("next_game") != std::string::npos)
        {
            _selected_y = _selected_game + 1;
            if (_selected_y > (int) _game_libs.size() - 1)
                _selected_y = 0;
            change_game();
        }

        if (ret.find("prev_game") != std::string::npos)
        {
            _selected_y = _selected_game - 1;
            if (_selected_y < 0)
                _selected_y = (int) _game_libs.size() - 1;
            change_game();
        }
        return ret;
    }
    else ret = lib->getPressedKey();
    _pressed += ret;
    if (_pressed.find("exit") != std::string::npos) {
        lib->destroy_screen();
        delete lib;
        dlclose(_handle_gr);
        return ret;
    }
    change_selected();
    if (ret == "enter") {
        if (_selected_x == 2)
            change_game();
        else if (_selected_x == 1)
            change_graphics();
    }
    lib->clear_screen();
    draw_all();
    return _pressed;
}

void Menu::draw_all()
{
    for (auto &game_name : _game_libs)
    {
        lib->draw(game_name.third, game_name.second.x1, game_name.second.x2,
                  game_name.second.y1, game_name.second.y2);
    }
    for (auto &gr_name : _gr_libs)
    {
        lib->draw(gr_name.third, gr_name.second.x1, gr_name.second.x2,
                  gr_name.second.y1, gr_name.second.y2);
    }
    lib->draw(_arrow.third, _arrow.second.x1, _arrow.second.x2, _arrow
    .second.y1, _arrow.second.y2);
    lib->draw(_player.third, _player.second.x1, _player.second.x2, _player.second
    .y1, _player.second.y2);
    for (auto &headline : _headlines)
    {
        lib->draw(headline.third, headline.second.x1, headline.second.x2,
                  headline.second.y1, headline.second.y2);
    }
    for (auto &score : _scores)
    {
        lib->draw(score.third, score.second.x1, score.second.x2, score
        .second.y1, score.second.y2);
    }
    for (auto &name : _names)
    {
        lib->draw(name.third, name.second.x1, name.second.x2, name
            .second.y1, name.second.y2);
    }
    if (!_name.first.text.empty())
        lib->draw(_name.third, _player.second.x1 + 120, _player.second.x2 + 120,
            _player
        .second.y1, _player.second.y2);
    _pressed.clear();
    lib->display_screen();
}

void Menu::change_graphics()
{
    std::string path = "lib/arcade_" + _gr_libs[_selected_y].first.text + ".so";
    void *tmp_handle_gr = dlopen(path.c_str(), RTLD_LAZY);
    auto func = (IGraphics *(*)()) dlsym(tmp_handle_gr,
        "create_graphics");
    lib->destroy_screen();
    dlclose(_handle_gr);
    _handle_gr = tmp_handle_gr;
    lib = (func)();
    lib->init_screen(1920, 1080);
    reload_all_objects();
    _selected_gr = _selected_y;

    if (_game != nullptr)
    {
        _game->use_graphics_lib(lib);
        _game->reload_all_objects();
    }
}

void Menu::change_game()
{
    std::ofstream is;
    is.open("users");
    is << _name.first.text;
    is.close();
    std::string path = "lib/arcade_" + _game_libs[_selected_y].first.text + ".so";
    transform(path.begin(), path.end(), path.begin(), ::tolower);
    _handle_game = dlopen(path.c_str(), RTLD_LAZY);
    auto func = (IGame *(*)(IGraphics *)) dlsym(_handle_game, "create_game");
    _game = (func)(lib);
    lib->destroy_screen();
    _game->start_game();
    _selected_game = _selected_y;
}

std::string &Menu::moveGame(std::string &ret)
{
    ret = _game->game_tick();
    _pressed += ret;
    if (ret == "m")
    {
        delete _game;
        _game = nullptr;
        dlclose(_handle_game);
        reload_all_objects();
    }
    if (_pressed.find("exit") != std::string::npos)
    {
        lib->destroy_screen();
        delete _game;
        _game = nullptr;
        delete lib;
        dlclose(_handle_game);
        dlclose(_handle_gr);
    }
    return ret;
}

void Menu::change_selected()
{
    if (_pressed.find("tab") != std::string::npos)
    {
        _selected_y = 0;
        _selected_x++;
        if (_selected_x > 2)
            _selected_x = 0;
    }
    move_up_down();
    if (_selected_x == 0)
    {
        _arrow.second = _player.second;
        _arrow.second.x1 -= 50;
        _arrow.second.x2 -= 50;
        if (_pressed.find("backspace") != std::string::npos && !_name.first.text.empty())
            _name.first.text = _name.first.text.substr(0, _name.first.text.size() - 1);
        else if (_pressed != "*" && _pressed.find("backspace")
        && _pressed.size() == 1 && isalnum(_pressed.c_str()[0]))
            _name.first.text += _pressed;
        else if (_pressed == "up")
            _name.first.text += "A";
        else if (_pressed == "down")
            _name.first.text += "B";
        else if (_pressed == "right")
            _name.first.text += "C";
        else if (_pressed == "left")
            _name.first.text += "D";
        else if (_pressed == "prev_g")
            _name.first.text += "o";
        else if (_pressed == "next_g")
            _name.first.text += "p";
        else if (_pressed == "prev_game")
            _name.first.text += "k";
        else if (_pressed == "next_game")
            _name.first.text += "l";
        if (!_pressed.empty())
        {
            lib->deleteObject(_name.third);
            _name.third = lib->createObject(&_name.first);
        }
    }
    if (_selected_x == 1)
    {
        _arrow.second = _gr_libs[0].second;
        _arrow.second.x1 = 150 / (int)_gr_libs.size() * _selected_y * 10;
        _arrow.second.x2 = 150 / (int)_gr_libs.size() * _selected_y * 10;
    }
    if (_selected_x == 2)
    {
        _arrow.second = _game_libs[_selected_y].second;
        _arrow.second.x1 -= 50;
        _arrow.second.x2 -= 50;
    }
}

void Menu::move_up_down()
{
    if (_pressed.find("right") != std::string::npos)
    {
        if (_selected_x != 0)
            _selected_y++;
        if ((_selected_x == 2 && _selected_y > (int) _game_libs.size() - 1) ||
            (_selected_x == 1 && _selected_y > (int) _gr_libs.size() - 1))
            _selected_y = 0;
    }
    if (_pressed.find("left") != std::string::npos)
    {
        if (_selected_x != 0)
            _selected_y--;
        if (_selected_y < 0)
        {
            if (_selected_x == 2) _selected_y = (int) _game_libs.size() - 1;
            else _selected_y = (int) _gr_libs.size() - 1;
        }
    }
}

bool Menu::is_running() {
    return _isRunning;
}

void Menu::use_graphics_lib(IGraphics *_lib)
{
    lib = _lib;
}

void Menu::set_handler(void *hd)
{
    _handle_gr = hd;
}

void Menu::InsertGameScores(std::string game_name, int it)
{
    transform(game_name.begin(), game_name.end(), game_name.begin(),
        ::tolower);
    std::ifstream is(game_name + "_scores");
    if (!is.is_open())
        return;
    std::string line;
    int counter = 2;
    while (getline(is, line) && counter < 12)
    {
        std::string score;
        std::string name;
        std::stringstream ss (line);
        ss >> name >> score;

        object_creation_data score_data;
        score_data.color_name = "red";
        score_data.type = TEXT;
        score_data.path_to_resource = "assets/font.ttf";
        score_data.text = score;
        Triple<object_creation_data, pos, void *> triple_score;
        triple_score.first = score_data;
        triple_score.second = _game_libs[it].second;
        triple_score.second.x1 += 250;
        triple_score.second.x2 += 250;
        triple_score.second.y1 += counter * 2 * 20;
        triple_score.second.y2 += counter * 2 * 20;
        triple_score.third = lib->createObject(&score_data);
        _scores.emplace_back(triple_score);

        object_creation_data name_data;
        name_data.color_name = "red";
        name_data.type = TEXT;
        name_data.path_to_resource = "assets/font.ttf";
        name_data.text = name;
        Triple<object_creation_data, pos, void *> triple_name;
        triple_name.first = name_data;
        triple_name.second = _game_libs[it].second;
        triple_name.second.y1 += counter * 2 * 20;
        triple_name.second.y2 += counter * 2 * 20;
        triple_name.third = lib->createObject(&name_data);
        _names.emplace_back(triple_name);
        counter++;
    }
}

void Menu::reload_all_objects()
{
    _name.third = lib->createObject(&_name.first);
    for (auto &line : _headlines)
        line.third = lib->createObject(&line.first);
    for (auto &line : _scores)
        line.third = lib->createObject(&line.first);
    for (auto &line : _scores)
        line.third = lib->createObject(&line.first);
    for (auto &line : _names)
        line.third = lib->createObject(&line.first);
    for (auto &line : _gr_libs)
        line.third = lib->createObject(&line.first);
    for (auto &line : _game_libs)
        line.third = lib->createObject(&line.first);
    _arrow.third = lib->createObject(&_arrow.first);
    _player.third = lib->createObject(&_player.first);
}
