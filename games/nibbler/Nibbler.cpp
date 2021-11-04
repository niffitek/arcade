/*
** EPITECH PROJECT, 2021
** B_OOP_400_BER_4_1_arcade_adrian_lewitzki
** File description:
** Nibbler.cpp
*/

#include <fstream>
#include <sstream>
#include <ostream>
#include "Nibbler.hpp"

extern "C" {
IGame *create_game(IGraphics *lib) {
    return new Nibbler(lib);
}
}

Nibbler::Nibbler(IGraphics *_lib)
{
    lib = _lib;
}

void Nibbler::start_game()
{
    _fruit.third = nullptr;
    lib->init_screen(1920, 1080);
    _gameover_screen1.first.text = "Game Over";
    _gameover_screen1.first.type = TEXT;
    _gameover_screen1.first.color_name = "red";
    _gameover_screen1.first.path_to_resource = "assets/font.ttf";
    _gameover_screen1.third = lib->createObject(&_gameover_screen1.first);

    _gameover_screen2.first.text = "Press enter to restart";
    _gameover_screen2.first.type = TEXT;
    _gameover_screen2.first.color_name = "green";
    _gameover_screen2.first.path_to_resource = "assets/font.ttf";
    _gameover_screen2.third = lib->createObject(&_gameover_screen2.first);
    boarder.second = {100, 800, 100, 900};
    boarder.first.text = "x";
    boarder.first.type = SPRITE;
    boarder.first.color_name = "white";
    boarder.first.path_to_resource = "assets/block.png";
    boarder.third = lib->createObject(&boarder.first);

    restart_game();
    _name.first.color_name = "red";
    _name.first.type = TEXT;
    _name.first.path_to_resource = "assets/font.ttf";
    _isRunning = true;
    std::ifstream os;
    os.open("users");
    if (os.is_open())
        getline(os, _name.first.text);
    _name.first.text = "name: " + _name.first.text;
    _name.second = lib->createObject(&_name.first);
    os.close();

    std::ifstream is;
    is.open("nibbler_scores");
    if (is.is_open())
    {
        while (1)
        {
            std::string line;
            if (getline(is, line))
            {
                std::stringstream tmp(line);
                std::string sc;
                std::string na;
                tmp >> na >> sc;
                try
                {
                    std::pair<std::string, int> tpair{na, stoi(sc)};
                    _scores.emplace_back(tpair);
                } catch (std::exception &)
                {
                }
            }
            else break;
        }
    }
}

void Nibbler::create_start_snake()
{
    for (auto &tmp : _snake)
        lib->deleteObject(&tmp.third);
    _snake.clear();
    for (auto &tmp : shits)
        lib->deleteObject(&tmp.third);
    shits.clear();
    Triple<object_creation_data, pos, void *>snake_head;
    snake_head.first.text = "x";
    snake_head.first.color_name = "red";
    snake_head.first.type = SPRITE;
    snake_head.first.path_to_resource = "assets/head_snake.png";
    snake_head.third = lib->createObject(&snake_head.first);
    snake_head.second = {600, 620, 600, 620};
    _snake.emplace_back(snake_head);
    make_longer(600, 620);
    make_longer(600, 640);
    make_longer(600, 660);
}

void Nibbler::create_fruit()
{
    _last_fruit = std::chrono::steady_clock::now();
    lib->deleteObject(_fruit.third);
    _fruit.first.text = "x";
    if (rand() % 2 == 0)
    {
        _fruit.first.color_name = "yellow";
        _fruit.first.path_to_resource = "assets/Bananas.png";
    }
    else
    {
        _fruit.first.color_name = "green";
        _fruit.first.path_to_resource = "assets/apple.png";
    }
    _fruit.first.type = SPRITE;
    _fruit.third = lib->createObject(&_fruit.first);

    _fruit.second.x1 = rand() % ((boarder.second.x2 - boarder.second.x1) /
        20) * 20 + boarder.second.x1;
    _fruit.second.x2 = _fruit.second.x1 + 20;
    _fruit.second.y1 = rand() % ((boarder.second.y2 - boarder.second.y1) /
        20) * 20 + boarder.second.y1;
    _fruit.second.y2 = _fruit.second.y1 + 20;
    if (check_collision())
        create_fruit();
}

std::string Nibbler::game_tick()
{
    _direction += lib->getPressedKey();
    if (_direction.find("exit") != std::string::npos)
    {
        lib->destroy_screen();
        return "exit";
    }
    if (_direction.find("enter") != std::string::npos)
    {
        restart_game();
    }
    if (_direction.find("space") != std::string::npos)
    {
        _fast = 1;
    }
    else
        _fast = 0;
    if (!_isRunning)
    {
        print_gameover();
        return _direction;
    }
    std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t -
    _last_tick).count();
    if (elapsed > 200000 - 200000 * _fast - score * 1000)
    {
        change_move();
        _last_tick = t;
        move_snake();
        check_fruit();
        _direction.clear();
        lib->clear_screen();
        col_end();
        draw_boarders();
        if (_isRunning)
        {
            for (auto &object : _snake)
                lib->draw(object.third, object.second.x1, object.second.x2,
                          object.second.y1, object.second.y2);
            for (auto &object : shits)
                lib->draw(object.third, object.second.x1, object.second.x2,
                          object.second.y1, object.second.y2);

            lib->draw(_fruit.third, _fruit.second.x1, _fruit.second.x2,
                      _fruit.second.y1, _fruit.second.y2);
        }
        lib->draw(score_data.second, 0, 10, 20, 400);
        lib->draw(_name.second, 0, 10, 0, 200);
        lib->display_screen();
        if (_shit)
        {
            Triple<object_creation_data, pos, void *> shit_tmp;
            shit_tmp.second = _snake[_snake.size() - 1].second;
            shit_tmp.first.type = SPRITE;
            shit_tmp.first.text = 'x';
            shit_tmp.first.path_to_resource = "assets/shit.png";
            shit_tmp.first.color_name = "magenta";
            shit_tmp.third = lib->createObject(&shit_tmp.first);
            shits.emplace_back(shit_tmp);
            _shit = false;
        }
    }
    return _direction;
}

void Nibbler::move_snake()
{
    pos prev;
    pos tmp;
    for (auto object = _snake.begin(); object != _snake.end(); object++)
    {
        tmp = object->second;
        if (object == _snake.begin())
        {
            prev = object->second;
            object->second.x1 += _mov.first * 10;
            object->second.x2 += _mov.first * 10;
            object->second.y1 += _mov.second * 20;
            object->second.y2 += _mov.second * 20;
        }
        else
        {
            object->second = prev;
            prev = tmp;
        }
    }
    _last_pos.emplace_back(tmp);
    if (_last_pos.size() > 2)
        _last_pos.erase(_last_pos.begin());
}

void Nibbler::col_end()
{
    for (auto object = _snake.begin(); object != _snake.end(); object++)
    {
        if (object->second.x1 <= boarder.second.x1 || object->second.x2 >
        boarder.second.x2 || object->second.y1 <= boarder.second.y1 ||
        object->second.y1 >= boarder.second.y2)
        {
            _isRunning = false;
            break;
        }
        for (auto obj = object; obj != _snake.end(); )
        {
            obj++;
            if (obj == _snake.end())
                break;
            if (object->second.x1 == obj->second.x1 &&
                object->second.y1 == obj->second.y1)
                _isRunning = false;
        }
    }
    for (auto object = shits.begin(); object != shits.end(); object++)
    {
        if (object->second.x1 == _snake.begin()->second.x1 &&
            object->second.y1 == _snake.begin()->second.y1)
        {
            _isRunning = false;
            break;
        }
    }
    if (!_isRunning)
    {
        std::ofstream os;
        os.open("nibbler_scores");
        int counter = 0;
        bool printed = false;
        for (auto &sc : _scores)
        {
            if (score > sc.second && counter <= 10 && !printed)
            {
                printed = true;
                std::pair<std::string, int>pa(_name.first.text.substr(6, _name.first.text.size() - 6), score);
                _scores.insert(_scores.begin() + counter, pa);
            }
            counter++;
        }
        if (counter <= 10 && !printed)
        {
            std::pair<std::string, int>pa(_name.first.text.substr(6, _name.first.text.size() - 6), score);
            _scores.insert(_scores.begin() + counter, pa);
        }
        counter = 0;
        for (auto &sc : _scores)
        {
            if (counter < 10)
                os << sc.first << " " << sc.second << std::endl;
        }
        score = 0;
        os.close();
    }
}

void Nibbler::check_fruit()
{
    std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t -
        _last_fruit).count();
    if (elapsed > 5000000)
        create_fruit();
    if (check_collision())
    {
        if (_fruit.first.color_name == "green")
            score += 10;
        else
            score += 5;
        score_data.first.text = "Score: " + std::to_string(score);
        lib->deleteObject(score_data.second);
        score_data.second = lib->createObject(&score_data.first);
        create_fruit();
        make_longer(_last_pos[_last_pos.size()].x1, _last_pos[_last_pos.size
        ()].y1);
        if (rand() % 2 == 0)
            make_longer(_last_pos[0].x1, _last_pos[0].y1);
        _shit = true;
    }
}

bool Nibbler::is_running()
{
    return true;
}

void Nibbler::change_move()
{
    if (_direction.find("right") != std::string::npos)
    {
        if (_cur_direction == LEFT)
            _cur_direction = UP;
        else
            _cur_direction++;
        _direction.clear();
    }
    if (_direction.find("left") != std::string::npos)
    {
        if (_cur_direction == UP)
            _cur_direction = LEFT;
        else
            _cur_direction--;
        _direction.clear();
    }
    if (_cur_direction == UP)
    {
        _mov.first = 0;
        _mov.second = -1;
    }
    else if (_cur_direction == DOWN)
    {
        _mov.first = 0;
        _mov.second = 1;
    }
    else if (_cur_direction == RIGHT)
    {
        _mov.first = 2;
        _mov.second = 0;
    }
    else if (_cur_direction == LEFT)
    {
        _mov.first = -2;
        _mov.second = 0;
    }
}

bool Nibbler::check_collision()
{
    pos sn;
    for (auto & it : _snake)
    {
        sn = it.second;
        if (sn.y1 == _fruit.second.y1 && sn.x1 == _fruit.second.x1)
            return true;
    }
    for (auto & it : shits)
    {
        sn = it.second;
        if (sn.y1 == _fruit.second.y1 && sn.x1 == _fruit.second.x1)
            return true;
    }
    return false;
}

void Nibbler::make_longer(int x, int y)
{
    Triple<object_creation_data , pos, void *> snake_add;
    snake_add.first.text = "x";
    snake_add.first.color_name = "blue";
    snake_add.first.type = object_type::SPRITE;
    snake_add.first.path_to_resource = "assets/block_snake.png";

    snake_add.second = {x, x + 20, y, y + 20};
    snake_add.third = lib->createObject(&snake_add.first);
    _snake.emplace_back(snake_add);
}

void Nibbler::print_gameover()
{
    std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
        t - _last_tick).count();
    if (elapsed > 200000)
    {
        lib->draw(_gameover_screen1.third, 200, 200, 560, 560);
        lib->draw(_gameover_screen2.third, 200, 200, 460, 460);
        lib->display_screen();
        _last_tick = std::chrono::steady_clock::now();
    }
}
void Nibbler::draw_boarders()
{

    for (int x = boarder.second.x1; x <= boarder.second.x2; x += 10)
    {
        lib->draw(boarder.third, x, x + 20, boarder.second.y1, boarder.second.y1 + 20);
        lib->draw(boarder.third, x, x + 20, boarder.second.y2, boarder.second.y2 + 20);
    }
    for (int y = boarder.second.y1; y <= boarder.second.y2; y += 10)
    {
        lib->draw(boarder.third, boarder.second.x1, boarder.second.x1 + 20, y, y + 20);
        lib->draw(boarder.third, boarder.second.x2, boarder.second.x2 + 20, y, y + 20);
    }
}

void Nibbler::restart_game()
{
    _last_tick = std::chrono::steady_clock::now();
    _isRunning = true;
    score = 0;
    score_data.first.text = "Score: 0";
    score_data.first.color_name = "yellow";
    score_data.first.type = TEXT;
    score_data.first.path_to_resource = "assets/font.ttf";
    score_data.second = lib->createObject(&score_data.first);

    create_start_snake();
    create_fruit();
    while (check_collision())
        create_fruit();
    _mov = {0, -1};
    _cur_direction = 0;
    _direction.clear();
}

void Nibbler::use_graphics_lib(IGraphics *_lib)
{
    lib = _lib;
}

void Nibbler::reload_all_objects()
{
    boarder.third = lib->createObject(&boarder.first);
    score_data.second = lib->createObject(&score_data.first);
    for (auto &line : _snake)
        line.third = lib->createObject(&line.first);
    for (auto &line : shits)
        line.third = lib->createObject(&line.first);
    _name.second = lib->createObject(&_name.first);
    _fruit.third = lib->createObject(&_fruit.first);
    _gameover_screen1.third = lib->createObject(&_gameover_screen1.first);
    _gameover_screen2.third = lib->createObject(&_gameover_screen2.first);
    _direction.clear();
}

Nibbler::~Nibbler()
{
    lib->deleteObject(boarder.third);
    lib->deleteObject(score_data.second);
    for (auto &line : _snake)
        lib->deleteObject(line.third);
    for (auto &line : shits)
        lib->deleteObject(line.third);
    lib->deleteObject(_name.second);
    lib->deleteObject(_fruit.third);
    lib->deleteObject(_gameover_screen1.third);
    lib->deleteObject(_gameover_screen2.third);
}

