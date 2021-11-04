/*
** EPITECH PROJECT, 2021
** B_OOP_400_BER_4_1_arcade_adrian_lewitzki
** File description:
** ncurses.hpp
*/

#ifndef B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_NCURSES_HPP
#define B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_NCURSES_HPP

#include "../IGraphics.hpp"
template<typename T1, typename  T2, typename  T3>
class Triple
{
public:
    T1 first;
    T2 second;
    T3 third;
};

class ncurses final : public IGraphics
{
private:
    int _width = 0;
    int _height = 0;
    bool _isValidWindow = true;
    static void changeColor(const std::string& color) ;
    static void reset_color();
    bool check_window_size();
    std::vector<Triple<std::string, std::string, object_type> *>
    _objs;
public:
    ~ncurses() final;
    void init_screen(int width, int height) final;
    void *createObject(object_creation_data *object_data) final;
    void draw(void *object, int x0, int x1, int y0, int y1) final;
    void deleteObject(void *object) final;
    std::string getPressedKey(void) final;
    void destroy_screen() final;
    void display_screen() final;
    void clear_screen() final;
};

extern "C" {
    IGraphics *create_graphics();
}

#endif //B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_NCURSES_HPP
