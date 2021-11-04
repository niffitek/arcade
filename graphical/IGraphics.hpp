/*
** EPITECH PROJECT, 2021
** B_OOP_400_BER_4_1_arcade_adrian_lewitzki
** File description:
** IGrafical.hpp
*/

#ifndef B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_IGRAPHICS_HPP
#define B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_IGRAPHICS_HPP

#include <string>
#include <vector>

enum object_type
{
    TEXT,
    SPRITE,
    RECT
};

typedef struct {
    object_type type;
    std::string text;
    std::string path_to_resource;
    std::string color_name;
} object_creation_data;

class IGraphics
{
public:
    virtual void init_screen(int width, int height) = 0;
    virtual ~IGraphics() = default;
    virtual void *createObject(object_creation_data *object_data) = 0;
    virtual void draw(void *object, int x0, int x1, int y0, int y1) = 0;
    virtual void deleteObject(void *object) = 0;
    virtual std::string getPressedKey(void) = 0;
    virtual void destroy_screen() = 0;
    virtual void display_screen() = 0;
    virtual void clear_screen() = 0;
};
#endif //B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_IGRAPHICS_HPP
