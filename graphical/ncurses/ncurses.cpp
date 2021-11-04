/*
** EPITECH PROJECT, 2021
** B_OOP_400_BER_4_1_arcade_adrian_lewitzki
** File description:
** ncurses.cpp
*/

#include "ncurses.hpp"
#include <ncurses.h>

extern "C" {
    IGraphics *create_graphics() {
        return new ncurses;
    }
}

void ncurses::init_screen(int width, int height)
{
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    _width = width / 10;
    _height = height / 10;
    timeout(100);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_BLUE, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(8, COLOR_BLACK, COLOR_WHITE);
}

void *ncurses::createObject(object_creation_data *data)
{
    Triple<std::string, std::string, object_type> * tmp = new
        Triple<std::string, std::string, object_type>;
    tmp->first = data->text;
    tmp->second = data->color_name;
    tmp->third = data->type;
    _objs.emplace_back(tmp);
    return tmp;
}

void ncurses::draw(void *object, int x0, int x1, int y0, int y1)
{
    Triple<std::string, std::string, object_type> *data = static_cast<Triple<std::string, std::string, object_type> *>(object);
    changeColor(data->second);
    if (data->third == SPRITE)
    {
        for (int i = y0 / 20; i < y1 / 20; i++)
        {
            for (int j = x0 / 10; j < x1 / 10; j += data->first.length())
            {
                if (data->first == "x")
                    mvaddch(i, j, ACS_CKBOARD);
                else
                    mvprintw(i, j, "%s", data->first.c_str());
            }
        }
    }
    else if (data->third == RECT)
    {
        for (int i = y0 / 20; i < y1 / 20; i++)
        {
            for (int j = x0 / 10; j < x1 / 10; j ++)
            {
                mvaddch(i, j, ACS_CKBOARD);
            }
        }
    }
    else
        mvprintw(y0 / 20, x0 / 10, "%s", data->first.c_str());
    reset_color();
}

void ncurses::changeColor(const std::string& color)
{
    if (color == "red")
        color_set(2, 0);
    else if (color == "green")
        color_set(3, 0);
    else if (color == "yellow")
        color_set(4, 0);
    else if (color == "blue")
        color_set(5, 0);
    else if (color == "magenta")
        color_set(7, 0);
    else if (color == "cyan")
        color_set(6, 0);
    else if (color == "black")
        color_set(8, 0);
    else
        color_set(1, 0);
}

void ncurses::reset_color()
{
    color_set(1, 0);
}

void ncurses::deleteObject(void *object)
{
    Triple<std::string, std::string, object_type> *data = static_cast<Triple<std::string, std::string, object_type> *>
    (object);
    for (size_t i = 0; i < _objs.size(); i += 1) {
        if (_objs[i] == data) {
            _objs.erase(_objs.begin() + i);
            delete data;
            break;
        }
    }
}

std::string ncurses::getPressedKey(void)
{
    std::string str;
    wchar_t c = getch();
    switch(c)
    {
        case '\n':
            str = "enter";
            break;
        case ' ':
            str = "space";
            break;
        case KEY_BACKSPACE:
            str = "backspace";
            break;
        case '\t':
            str = "tab";
            break;
        case KEY_EXIT:
            str = "esc";
            break;
        case 'q':
            str = "exit";
            break;
        case KEY_RIGHT:
            str = "right";
            break;
        case KEY_LEFT:
            str = "left";
            break;
        case KEY_UP:
            str = "up";
            break;
        case KEY_DOWN:
            str = "down";
            break;
        case 'o':
            str = "prev_gr";
            break;
        case 'p':
            str = "next_gr";
            break;
        case 'k':
            str = "prev_game";
            break;
        case 'l':
            str = "next_game";
            break;
        default:
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
            {
                str = c;
                break;
            }
            str = '*';
    }
    return str;
}

bool ncurses::check_window_size()
{
    int x = 0;
    int y = 0;
    getmaxyx(stdscr, y, x);
    if (x < _width || y < _width)
    {
        if (_isValidWindow)
        {
            mvprintw(y / 2, x / 2, "window too small");
        }
        _isValidWindow = false;
    }
    else
    {
        _isValidWindow = true;
    }
    return _isValidWindow;
}

void ncurses::destroy_screen()
{
    nocbreak();
    echo();
    endwin();
    curs_set(1);
}

void ncurses::clear_screen()
{
    clear();
}

void ncurses::display_screen()
{
    refresh();
}

ncurses::~ncurses()
{
    for (auto &del : _objs)
        delete del;
}
