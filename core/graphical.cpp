/*
** EPITECH PROJECT, 2019
** FILE_NAME
** File description:
** FILE DESCRIPTION
*/

#include "graphical.hpp"

GraphicalLib::GraphicalLib(const std::string &path)
{
    _handler = dlopen(path.c_str(), RTLD_LAZY);
    if (_handler == nullptr) {
        std::cerr << dlerror() << std::endl;
        exit(84);
    }
}

void *GraphicalLib::loadFunc(const char *name)
{
    void *func = dlsym(_handler, name);

    if (func == nullptr) {
        std::cerr << dlerror() << std::endl;
        exit(84);
    }
    return func;
}

void GraphicalLib::init_screen(int width, int height)
{
    void (*init_screen)(int, int);

    init_screen = reinterpret_cast<typeof init_screen>
        (loadFunc("init_screen"));
    return ((init_screen)(width, height));
}

void *GraphicalLib::createObject(object_creation_data *object_data)
{
    void *(*createObject)(void *);

    createObject = reinterpret_cast<typeof createObject>
        (loadFunc("createObject"));
    return ((createObject)(object_data));
}

void GraphicalLib::draw(void *object, int x0, int x1, int y0, int y1)
{
    void (*draw)(void *, int, int, int, int);

    draw = reinterpret_cast<typeof draw>(loadFunc("draw"));
    return ((draw)(object, x0, x1, y0, y1));
}

void GraphicalLib::deleteObject(void *object)
{
    void (*deleteObject)(void *);

    deleteObject = reinterpret_cast<typeof deleteObject>
        (loadFunc("deleteObject"));
    return ((deleteObject)(object));
}

std::string GraphicalLib::getPressedKey(void)
{
    std::string (*getPressedKey)();

    getPressedKey = reinterpret_cast<typeof getPressedKey>
        (loadFunc("getPressedKey"));
    return ((getPressedKey)());
}

void GraphicalLib::destroy_screen()
{
    void (*close_screen)();

    close_screen = reinterpret_cast<typeof close_screen>
        (loadFunc("destroy_screen"));
    return ((close_screen)());
}

void GraphicalLib::display_screen()
{
    void (*display_screen)();

    display_screen = reinterpret_cast<typeof display_screen>
        (loadFunc("display_screen"));
    return ((display_screen)());
}

void GraphicalLib::clear_screen()
{
    void (*clear_screen)();

    clear_screen = reinterpret_cast<typeof clear_screen>
        (loadFunc("clear_screen"));
    return ((clear_screen)());
}

