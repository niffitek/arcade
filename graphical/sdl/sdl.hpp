/*
** EPITECH PROJECT, 2019
** FILE_NAME
** File description:
** FILE DESCRIPTION
*/

#ifndef B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_SDL_HPP
#define B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_SDL_HPP

#include "../IGraphics.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <algorithm>
#include <vector>
#include <iostream>

struct Obj
{
    int height;
    int width;
    SDL_Color color;
    object_type type;
    void *toRender;
};

class sdl final : public IGraphics
{
private:
    int _width = 0;
    int _height = 0;
    std::vector<std::pair<std::string, SDL_Color>> _colors;
    SDL_Event _event;
    SDL_Renderer *_renderer;
    SDL_Window *_window;
    std::vector<Obj> _graphics;
    SDL_Color getColor(object_creation_data *data);
public:
    void init_screen(int width, int height) final;
    void *createObject(object_creation_data *data) final;
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

#endif //B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_SDL_HPP
