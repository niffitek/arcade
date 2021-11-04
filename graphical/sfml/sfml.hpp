/*
** EPITECH PROJECT, 2019
** FILE_NAME
** File description:
** FILE DESCRIPTION
*/

#ifndef B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_SFML_HPP
#define B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_SFML_HPP

#include "../IGraphics.hpp"
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <algorithm>

template<typename T1, typename  T2, typename T3>
class Triple
{
public:
    T1 first;
    T2 second;
    T3 third;
};

class sfml final : public IGraphics
{
private:
    int _width = 0;
    int _height = 0;
    sf::RenderWindow _window;
    std::vector<Triple<object_creation_data, object_type, void *>*>_objects;

    std::string getKey(sf::Event ev);
    sf::Vector2f calcScale(const sf::Sprite &sprite, int x0, int x1, int y0,
                           int y1);
    sf::Color getColor(std::string colorcode);
public:
    ~sfml() final;
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

#endif //B_OOP_400_BER_4_1_ARCADE_ADRIAN_LEWITZKI_SFML_HPP
