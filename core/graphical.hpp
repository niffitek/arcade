/*
** EPITECH PROJECT, 2019
** FILE_NAME
** File description:
** FILE DESCRIPTION
*/

#ifndef B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_GRAPHICAL_HPP
#define B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_GRAPHICAL_HPP

#include <string>
#include <iostream>
#include <dlfcn.h>

#include "../graphical/IGraphics.hpp"

class GraphicalLib final : public IGraphics
{
protected:
    void *_handler;
public:
    GraphicalLib(const std::string &path);
    void init_screen(int width, int height) final;
    void *createObject(object_creation_data *object_data) final;
    void draw(void *object, int x0, int x1, int y0, int y1) final;
    void deleteObject(void *object) final;
    std::string getPressedKey(void) final;
    void destroy_screen() final;
    void display_screen() final;
    void clear_screen() final;
    void *loadFunc(const char *name);
};

#endif //B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_GRAPHICAL_HPP
