/*
** EPITECH PROJECT, 2019
** FILE_NAME
** File description:
** FILE DESCRIPTION
*/

#include "Solarfox.hpp"
#include <utility>

extern "C" {
IGame *create_game(IGraphics *lib) {
    return new Solarfox(lib);
}
}

Solarfox::Solarfox(IGraphics *lib) : lib(lib)
{
}

void Solarfox::start_game()
{
    lib->init_screen(1920, 1080);
    _name.first = fill_data("", TEXT, "assets/font.ttf", "red");
    _isRunning = true;
    _created_clocks = false;
    _start_tick = std::chrono::steady_clock::now();
    std::ifstream os;
    os.open("users");
    if (os.is_open())
        getline(os, _name.first.text);
    _name.first.text = "name: " + _name.first.text;
    _name.second = lib->createObject(&_name.first);
    os.close();

    std::ifstream is;
    is.open("solarfox_scores");
    if (is.is_open()) {
        std::string line;
        while (getline(is, line)) {
            std::stringstream tmp(line);
            std::string sc;
            std::string na;
            tmp >> na >> sc;
            try {
                std::pair<std::string, int> tpair{na, stoi(sc)};
                _scores.emplace_back(tpair);
            } catch (std::exception &) {}
        }
    }
    create_borders();
    create_grid();
    create_player();
    create_enemies();
    create_powerups();
    create_score_data();
    create_game_over();
    _objects.insert(std::pair<std::string, std::vector<object_triple>>
                        ("my_projectiles", std::vector<object_triple>()));
    _objects.insert(std::pair<std::string, std::vector<object_triple>>
    ("enemy_projectiles", std::vector<object_triple>()));
}

void Solarfox::create_score_data()
{
    _score_data.first.text = "Score: 0";
    _score_data.first.color_name = "yellow";
    _score_data.first.path_to_resource = "assets/font.ttf";
    _score_data.first.type = TEXT;
    _score_data.third = 0;
    _score_data.second = lib->createObject(&_score_data.first);
}

std::string Solarfox::game_tick()
{
    _input += lib->getPressedKey();

    if (_input.find("exit") != std::string::npos) {
        lib->destroy_screen();
        return "exit";
    }
    if (_input.find("enter") != std::string::npos)
        restart_game();
    if (!_isRunning) {
        game_over();
        return _input;
    }
    std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t -
        _last_tick).count();
    if (_input == "a" && elapsed > 50000) {
        _last_tick = t;
        get_move();
        move_player();
        lib->draw(_player.third, _player.second.x1, _player.second.x2,
                  _player.second.y1, _player.second.y2);
    }
    if (elapsed > 100000) {
        shot();
        get_move();
        move_player();
        move_enemies();
        check_powerups();
        move_enemy_projectile();
        move_my_projectile();
        check_projectile_col();
        _last_tick = t;
        _input.clear();
        lib->clear_screen();
        lib->clear_screen();
        if (!_created_clocks)
        {
            create_enemy_clocks();
        }
        for (auto &object : _objects["border"])
        {
            lib->draw(object.third, object.second.x1, object.second.x2,
                      object.second.y1, object.second.y2);
        }
        for (auto &object : _objects["grid"])
        {
            lib->draw(object.third, object.second.x1, object.second.x2,
                      object.second.y1, object.second.y2);
        }
        for (auto &object : _objects["enemies"])
        {
            lib->draw(object.third, object.second.x1, object.second.x2,
                      object.second.y1, object.second.y2);
        }
        for (auto &object : _objects["enemy_projectiles"])
        {
            lib->draw(object.third, object.second.x1, object.second.x2,
                      object.second.y1, object.second.y2);
        }
        for (auto &object : _objects["powerups"])
        {
            lib->draw(object.third, object.second.x1, object.second.x2,
                      object.second.y1, object.second.y2);
        }
        for (auto &object : _objects["my_projectiles"])
        {
            lib->draw(object.third, object.second.x1, object.second.x2,
                      object.second.y1, object.second.y2);
        }
        lib->draw(_player.third, _player.second.x1, _player.second.x2,
                  _player.second.y1, _player.second.y2);
        for (unsigned int i = 0; i < _last_enemy_shot.size(); i += 1)
        {
            auto enemy_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
                t - _last_enemy_shot[i]).count();
            if (_created_clocks && enemy_elapsed > 6000000)
            {
                _last_enemy_shot[i] = t;
                create_enemy_projectile(i);
            }
        }
        lib->draw(_score_data.second, 0, 20, 20, 40);
        lib->draw(_name.second, 0, 20, 0, 20);
        lib->display_screen();
    }
    return _input;
}

bool Solarfox::is_running()
{
    return _isRunning;
}

void Solarfox::use_graphics_lib(IGraphics *_lib)
{
    lib = _lib;
}

void Solarfox::reload_all_objects()
{
    for (auto &object : _objects["border"])
        object.third = lib->createObject(&object.first);
    for (auto &object : _objects["grid"])
        object.third = lib->createObject(&object.first);
    for (auto &object : _objects["enemies"])
        object.third = lib->createObject(&object.first);
    for (auto &object : _objects["enemy_projectiles"])
        object.third = lib->createObject(&object.first);
    for (auto &object : _objects["powerups"])
        object.third = lib->createObject(&object.first);
    for (auto &object : _objects["my_projectiles"])
        object.third = lib->createObject(&object.first);
    _player.third = lib->createObject(&_player.first);
    _game_over_text.first.third = lib->createObject(&_game_over_text.first
        .first);
    _game_over_text.second.third = lib->createObject(&_game_over_text.second
        .first);
    _name.second = lib->createObject(&_name.first);
    _score_data.second = lib->createObject(&_score_data.first);
    _input.clear();
}

void Solarfox::create_game_over()
{
    object_creation_data tmp = fill_data("Game Over",
                                          TEXT, "assets/font.ttf", "red");
    void *tmp_obj = lib->createObject(&tmp);
    pos tmp_pos = {200, 200, 500, 500};
    object_creation_data tmp2 = fill_data("Press enter to restart", TEXT,
                                           "assets/font.ttf", "green");
    void *tmp_obj2 = lib->createObject(&tmp2);
    pos tmp_pos2 = {200, 200, 560, 560};
    _game_over_text.first = create_triple(tmp, tmp_pos, tmp_obj);
    _game_over_text.second = create_triple(tmp2, tmp_pos2, tmp_obj2);
}

void Solarfox::game_over()
{
    lib->clear_screen();
    lib->draw(_game_over_text.first.third, 200, 200, 500, 500);
    lib->draw(_game_over_text.second.third, 200, 200, 560, 560);
    lib->display_screen();
}

void Solarfox::restart_game()
{
    _isRunning = true;
    _created_clocks = false;
    _cur_direction = 0;
    _next_direction = 0;
    _start_tick = std::chrono::steady_clock::now();
    _objects["enemy_projectiles"].clear();
    _objects["powerups"].clear();
    _objects.erase("powerups");
    _objects["my_projectiles"].clear();
    _move_enemy.clear();
    _move_enemy_projectile.clear();
    _move_my_projectile.clear();
    _last_enemy_shot.clear();
    create_player();
    create_powerups();
    create_score_data();
}

void Solarfox::create_borders()
{
    object_creation_data border_data = fill_data("x", SPRITE,
                                                  "assets/border.png",
                                            "white");
    void *border_obj = lib->createObject(&border_data);
    std::vector<object_triple> tmp_vec;
    for (int x = border.x1; x <= border.x2; x += 10)
    {
        pos tmp1 = {x, (x + 10), border.y1, (border.y1 + 20)};
        pos tmp2 = {x, (x + 10), border.y2, (border.y2 + 20)};
        tmp_vec.emplace_back(create_triple(border_data, tmp1, border_obj));
        tmp_vec.emplace_back(create_triple(border_data, tmp2, border_obj));
    }
    for (int y = border.y1; y <= border.y2; y += 10)
    {
        pos tmp1 = {border.x1, border.x1 + 10, y, y + 20};
        pos tmp2 = {border.x2, border.x2 + 10, y, y + 20};
        tmp_vec.emplace_back(create_triple(border_data, tmp1, border_obj));
        tmp_vec.emplace_back(create_triple(border_data, tmp2, border_obj));
    }
    _objects.insert(std::pair<std::string, std::vector<object_triple>>("border", tmp_vec));
}

object_creation_data 
Solarfox::fill_data(std::string text, object_type type, std::string path,
                    std::string color)
                    {
    object_creation_data data;
    data.text = std::move(text);
    data.type = type;
    data.path_to_resource = std::move(path);
    data.color_name = std::move(color);
    return data;
}

void Solarfox::create_grid()
{
    object_creation_data vertical_data = fill_data("|", SPRITE,
                                              "assets/vertical.png", "white");
    void *vertical = lib->createObject(&vertical_data);
    object_creation_data horizontal_data = fill_data("-", SPRITE,
                                                "assets/horizontal.png",
                                                "white");
    void *horizontal = lib->createObject(&horizontal_data);
    std::vector<object_triple> tmp_vec;
    for (int y = border.y1 + 80; y < border.y2; y += 80) {
        for (int x = border.x1 + 10; x < border.x2; x += 10) {
        pos tmp = {x, x + 10, y, y + 20};
        tmp_vec.emplace_back(create_triple(horizontal_data, tmp, horizontal));
        }
    }
    for (int x = border.x1 + 80; x < border.x2; x += 80) {
        for (int y = border.y1 + 20; y < border.y2; y += 10) {
            pos tmp = {x, x + 10, y, y + 20};
            tmp_vec.emplace_back(create_triple(vertical_data, tmp, vertical));
        }
    }
    _objects.insert(std::pair<std::string, std::vector<object_triple>>("grid",
        tmp_vec));
}

object_triple
Solarfox::create_triple(object_creation_data data, pos position, void *
object) {
    object_triple t;
    t.first = data;
    t.second = position;
    t.third = object;
    return t;
}

void Solarfox::create_player()
{
    object_creation_data player_data = fill_data("x", SPRITE,
                                                    "assets/player.png",
                                                    "blue");
    pos player_pos = {500, 520, 500, 520};
    void *player_obj = lib->createObject(&player_data);
    _player = create_triple(player_data, player_pos, player_obj);
}

void Solarfox::move_player()
{
    _player.second.x1 += _move_vec.first * 10;
    _player.second.x2 += _move_vec.first * 10;
    _player.second.y1 += _move_vec.second * 10;
    _player.second.y2 += _move_vec.second * 10;
}

void Solarfox::get_move()
{
    if (_input.find("right") != std::string::npos && _cur_direction != LEFT)
        _next_direction = RIGHT;
    if (_input.find("left") != std::string::npos && _cur_direction != RIGHT)
        _next_direction = LEFT;
    if (_input.find("up") != std::string::npos && _cur_direction != DOWN)
        _next_direction = UP;
    if (_input.find("down") != std::string::npos && _cur_direction != UP)
        _next_direction = DOWN;
    check_area();
    if (_cur_direction == UP)
    {
        _move_vec.first = 0;
        _move_vec.second = -1;
    }
    else if (_cur_direction == DOWN)
    {
        _move_vec.first = 0;
        _move_vec.second = 1;
    }
    else if (_cur_direction == RIGHT)
    {
        _move_vec.first = 1;
        _move_vec.second = 0;
    }
    else if (_cur_direction == LEFT)
    {
        _move_vec.first = -1;
        _move_vec.second = 0;
    }
}

void Solarfox::check_area()
{
    for (auto &object : _objects["grid"]) {
        if (_cur_direction == LEFT || _cur_direction == RIGHT) {
            if (object.first.text == "|" && object.second.x1 == _player
            .second.x1 && object.second.y1 == _player.second.y1)
                _cur_direction = _next_direction;
        }
        if (_cur_direction == UP || _cur_direction == DOWN) {
            if (object.first.text == "-" && object.second.x1 == _player
                .second.x1 && object.second.y1 == _player.second.y1)
                _cur_direction = _next_direction;
        }
    }
    if (_player.second.x1 <= 120 || _player.second.x1 >= 880 || _player
    .second.y1 <= 120 || _player.second.y1 >= 780)
    {
        safe_score();
        _isRunning = false;
    }
}

void Solarfox::create_enemies()
{
    object_creation_data enemy_data = fill_data("x", SPRITE,
                                                  "assets/enemy.png",
                                                  "red");
    void *enemy_obj = lib->createObject(&enemy_data);
    pos pos1 = {500, 520, 100, 120};
    pos pos2 = {500, 520, 800, 820};
    pos pos3 = {100, 120, 500, 520};
    pos pos4 = {900, 920, 500, 520};
    std::vector<object_triple> tmp_vec;

    tmp_vec.emplace_back(create_triple(enemy_data, pos1, enemy_obj));
    tmp_vec.emplace_back(create_triple(enemy_data, pos2, enemy_obj));
    tmp_vec.emplace_back(create_triple(enemy_data, pos3, enemy_obj));
    tmp_vec.emplace_back(create_triple(enemy_data, pos4, enemy_obj));
    _objects.insert(std::pair<std::string, std::vector<object_triple>>("enemies",
        tmp_vec));
    _move_enemy.emplace_back(1);
    _move_enemy.emplace_back(-1);
    _move_enemy.emplace_back(1);
    _move_enemy.emplace_back(-1);
}

void Solarfox::move_enemies()
{
    _objects["enemies"][0].second.x1 += _move_enemy[0] * 10;
    _objects["enemies"][0].second.x2 += _move_enemy[0] * 10;
    _objects["enemies"][1].second.x1 += _move_enemy[1] * 10;
    _objects["enemies"][1].second.x2 += _move_enemy[1] * 10;
    _objects["enemies"][2].second.y1 += _move_enemy[2] * 10;
    _objects["enemies"][2].second.y2 += _move_enemy[2] * 10;
    _objects["enemies"][3].second.y1 += _move_enemy[3] * 10;
    _objects["enemies"][3].second.y2 += _move_enemy[3] * 10;
    if (_objects["enemies"][0].second.x1 <= 120 || _objects["enemies"][0]
    .second.x1 >= 880)
        _move_enemy[0] *= -1;
    if (_objects["enemies"][1].second.x1 <= 120 || _objects["enemies"][1]
    .second.x1 >= 880)
        _move_enemy[1] *= -1;
    if (_objects["enemies"][2].second.y1 <= 120 || _objects["enemies"][2]
    .second.y1 >= 780)
        _move_enemy[2] *= -1;
    if (_objects["enemies"][3].second.y1 <= 120 || _objects["enemies"][3]
    .second.y1 >= 780)
        _move_enemy[3] *= -1;
}

void Solarfox::create_enemy_projectile(unsigned int enemy)
{
    std::pair<int, int> tmp;
    object_creation_data projectile_data;
    if (enemy == 0)
    {
        projectile_data = fill_data("x", SPRITE,
                                            "assets/enemy_shot_down.png",
                                                         "magenta");
        tmp.first = 0;
        tmp.second = 1;
    }
    else if (enemy == 1)
    {
        projectile_data = fill_data("x", SPRITE,
                                             "assets/enemy_shot_up.png",
                                                         "magenta");
        tmp.first = 0;
        tmp.second = -1;
    }
    else if (enemy == 2)
    {
        projectile_data = fill_data("x", SPRITE,
                                              "assets/enemy_shot_right.png",
                                                         "magenta");
        tmp.first = 1;
        tmp.second = 0;
    }
    else if (enemy == 3)
    {
        projectile_data = fill_data("x", SPRITE,
                                       "assets/enemy_shot_left.png",
                                                         "magenta");
        tmp.first = -1;
        tmp.second = 0;
    }
    pos projectile_pos = _objects["enemies"][enemy].second;
    if (rand() % 5 == 0)
    {
        projectile_pos.x1 -= 5;
        projectile_pos.x2 += 5;
        projectile_pos.y1 -= 5;
        projectile_pos.y2 += 5;
    }
    void *projectile_obj = lib->createObject(&projectile_data);
    _objects["enemy_projectiles"].emplace_back(create_triple(projectile_data,
                                                             projectile_pos, projectile_obj));
    _move_enemy_projectile.emplace_back(tmp);
}

void Solarfox::move_enemy_projectile()
{
    for (unsigned int i = 0; i < _objects["enemy_projectiles"].size(); i += 1) {
         _objects["enemy_projectiles"][i].second.x1 += _move_enemy_projectile[i]
             .first * 15;
        _objects["enemy_projectiles"][i].second.x2 += _move_enemy_projectile[i]
            .first * 15;
        _objects["enemy_projectiles"][i].second.y1 += _move_enemy_projectile[i]
            .second * 15;
        _objects["enemy_projectiles"][i].second.y2 += _move_enemy_projectile[i]
            .second * 15;
    }
}

void Solarfox::create_enemy_clocks()
{
    std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>
        (t - _start_tick).count();

    if (_last_enemy_shot.empty())
        _last_enemy_shot.emplace_back(std::chrono::steady_clock::now());
    if (_last_enemy_shot.size() == 1 && elapsed > 1500000)
        _last_enemy_shot.emplace_back(std::chrono::steady_clock::now());
    if (_last_enemy_shot.size() == 2 && elapsed > 3000000)
        _last_enemy_shot.emplace_back(std::chrono::steady_clock::now());
    if (_last_enemy_shot.size() == 3 && elapsed > 4500000) {
        _last_enemy_shot.emplace_back(std::chrono::steady_clock::now());
        _created_clocks = true;
    }
}

void Solarfox::check_projectile_col()
{
    for (unsigned int i = 0; i < _objects["enemy_projectiles"].size(); i += 1) {
        if (_objects["enemy_projectiles"][i].second.x1 <= 100 ||
        _objects["enemy_projectiles"][i].second.x1 >= 900 ||
        _objects["enemy_projectiles"][i].second.y1 <= 100 ||
        _objects["enemy_projectiles"][i].second.y1 >= 800) {
            _objects["enemy_projectiles"].erase(_objects["enemy_projectiles"]
            .begin() + i);
            _move_enemy_projectile.erase(_move_enemy_projectile.begin() + i);
        }
        if (_objects["enemy_projectiles"][i].second.x1 <= _player.second.x2
        && _objects["enemy_projectiles"][i].second.x2 >= _player.second.x1
        && _objects["enemy_projectiles"][i].second.y1 <= _player.second.y2
        && _objects["enemy_projectiles"][i].second.y2 >= _player.second.y1) {
            _isRunning = false;
            safe_score();
        }
    }
}

void Solarfox::create_powerups()
{
    auto fill = fill_data("x", SPRITE, "assets/apple.png", "green");
    auto fill_b = fill_data("x", SPRITE, "assets/shit.png", "magenta");
    std::vector<object_triple> tmp_tr;
    void *tmp_ptr = lib->createObject(&fill);
    void *tmp_ptr_b = lib->createObject(&fill_b);
    for (int i = border.x1 + 80; i < border.x2; i += 80)
    {
        for (int j = border.y1 + 80; j < border.y2; j += 80)
        {
            if ((i == 500 && j == 500))
                continue;
            pos tmp = {i, i + 20, j, j + 20};
            int r = rand() % 100;
            if (r > 90)
            {
                _objects["enemy_projectiles"].emplace_back(create_triple(fill_b, tmp, tmp_ptr_b));
                _move_enemy_projectile.emplace_back(std::pair<int, int>(0, 0));
            }
            else if (r > 33)
            {
                tmp_tr.emplace_back(create_triple(fill, tmp, tmp_ptr));
            }
        }
    }
    _objects.insert(std::pair<std::string, std::vector<object_triple>>
    ("powerups", tmp_tr));
}

void Solarfox::check_powerups()
{
    int pos_i = 0;
    if (_objects["powerups"].empty())
    {
        _objects["enemy_projectiles"].clear();
        _objects.erase("powerups");
        create_player();
        create_powerups();
    }
    for (auto &obj : _objects["powerups"])
    {
        if (obj.second.x1 <= _player.second.x2
            && obj.second.x2 >= _player.second.x1
            && obj.second.y1 <= _player.second.y2
            && obj.second.y2 >= _player.second.y1)
        {
            _score_data.third += 10;
            _score_data.first.text = "Score: " + std::to_string(_score_data.third);
            lib->deleteObject(_score_data.second);
            _score_data.second = lib->createObject(&_score_data.first);
            _objects["powerups"].erase(_objects["powerups"].begin() + pos_i);
        }
        pos_i++;
    }
}

void Solarfox::shot()
{
    if (_input.find("s") != std::string::npos)
    {
        std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t -
                                                                 _last_shot).count();
        if (elapsed > 500000)
        {
            _last_shot = t;
            object_triple tmp;
            tmp.first.type = SPRITE;
            tmp.first.text = "x";
            tmp.first.path_to_resource = "assets/my_shot.png";
            tmp.first.color_name = "yellow";
            tmp.third = lib->createObject(&tmp.first);
            tmp.second = _player.second;
            _objects["my_projectiles"].emplace_back(tmp);
            Triple<int, int, int> tr{_move_vec.first * 30,
                                     _move_vec.second * 30, 0};
            _move_my_projectile.emplace_back(tr);
        }
    }
}

void Solarfox::move_my_projectile()
{
    for (size_t i = 0; i < _move_my_projectile.size(); i++)
    {
        pos tmp = _objects["my_projectiles"][i].second;
        tmp.x1 += _move_my_projectile[i].first;
        tmp.x2 += _move_my_projectile[i].first;
        tmp.y1 += _move_my_projectile[i].second;
        tmp.y2 += _move_my_projectile[i].second;

        for (size_t j = 0; j < _objects["powerups"].size(); j++)
        {
            pos tm = _objects["my_projectiles"][i].second;
            pos obj = _objects["powerups"][j].second;
            if (tm.x1 <= obj.x2
                && tm.x2 >= obj.x1
                && tm.y1 <= obj.y2
                && tm.y2 >= obj.y1)
            {
                _score_data.third += 10;
                _score_data.first.text = "Score: " + std::to_string(_score_data.third);
                lib->deleteObject(_score_data.second);
                _score_data.second = lib->createObject(&_score_data.first);
                _objects["powerups"].erase(_objects["powerups"].begin() + j);
                _objects["my_projectiles"].erase(_objects["my_projectiles"]
                .begin() + i);
                _move_my_projectile.erase(_move_my_projectile.begin() + i);
                i--;
                goto con;
            }
        }
        for (size_t j = 0; j < _objects["enemy_projectiles"].size(); j++)
        {
            pos tm = _objects["my_projectiles"][i].second;
            pos obj = _objects["enemy_projectiles"][j].second;
            if (tm.x1 <= obj.x2
                && tm.x2 >= obj.x1
                && tm.y1 <= obj.y2
                && tm.y2 >= obj.y1)
            {
                _objects["enemy_projectiles"].erase(_objects["enemy_projectiles"].begin() + j);
                _objects["my_projectiles"].erase(_objects["my_projectiles"]
                                                     .begin() + i);
                _move_my_projectile.erase(_move_my_projectile.begin() + i);
                _move_enemy_projectile.erase(_move_enemy_projectile.begin() +
                i);
                i--;
                goto con;
            }
        }
        _objects["my_projectiles"][i].second = tmp;
        _move_my_projectile[i].third++;
        if (tmp.x1 <= border.x1 || tmp.y1 <= border.y1 || tmp.y2 >= border.y2
        || tmp.x2 >= border.x2 || _move_my_projectile[i].third > 5)
        {
            _objects["my_projectiles"].erase(_objects["my_projectiles"].begin
            () + i);
            _move_my_projectile.erase(_move_my_projectile.begin() + i);
            i--;
        }
        con:;
    }
}

void Solarfox::safe_score()
{
    std::ofstream os;
    os.open("solarfox_scores");
    int counter = 0;
    bool printed = false;
    for (auto &sc : _scores)
    {
        if (_score_data.third > sc.second && counter <= 10 && !printed)
        {
            printed = true;
            std::pair<std::string, int>pa(_name.first.text.substr(6, _name
            .first.text.size() - 6), _score_data.third);
            _scores.insert(_scores.begin() + counter, pa);
        }
        counter++;
    }
    if (counter <= 10 && !printed)
    {
        std::pair<std::string, int>pa(_name.first.text.substr(6, _name.first
        .text.size() - 6), _score_data.third);
        _scores.insert(_scores.begin() + counter, pa);
    }
    counter = 0;
    for (auto &sc : _scores)
    {
        if (counter < 10)
            os << sc.first << " " << sc.second << std::endl;
    }
    _score_data.third = 0;
    os.close();
}

Solarfox::~Solarfox()
{
    for (auto &object : _objects["border"])
        lib->deleteObject(&object.first);
    for (auto &object : _objects["grid"])
        lib->deleteObject(&object.first);
    for (auto &object : _objects["enemies"])
        lib->deleteObject(&object.first);
    for (auto &object : _objects["enemy_projectiles"])
        lib->deleteObject(&object.first);
    for (auto &object : _objects["powerups"])
        lib->deleteObject(&object.first);
    for (auto &object : _objects["my_projectiles"])
        lib->deleteObject(&object.first);
    lib->deleteObject(&_player.first);
    lib->deleteObject(&_game_over_text.first.first);
    lib->deleteObject(&_game_over_text.second.first);
    lib->deleteObject(&_name.first);
    lib->deleteObject(&_score_data.first);
}
