/*
** EPITECH PROJECT, 2019
** FILE_NAME
** File description:
** FILE DESCRIPTION
*/

#include "sfml.hpp"

extern "C" {
IGraphics *create_graphics() {
    return new sfml;
}
}

void sfml::init_screen(int width, int height)
{
    this->_window.create(sf::VideoMode(width, height), "Arcade");
}

void *sfml::createObject(object_creation_data *object_data)
{
    object_creation_data *data = static_cast<object_creation_data *>
    (object_data);
    Triple<object_creation_data, object_type, void *> * tmp = new
        Triple<object_creation_data, object_type, void *>;
    tmp->first = *data;
    if (data->type == SPRITE) {
        sf::Texture *texture = new sf::Texture;
        sf::Sprite *sprite = new sf::Sprite;

        if (!texture->loadFromFile(data->path_to_resource)) {
            std::cerr << "Loading sprite failed!" << std::endl;
            return nullptr;
        }
        sprite->setTexture(*texture);
        tmp->second = SPRITE;
        tmp->third = sprite;
    }
    if (data->type == TEXT)
    {
        sf::Font *font = new sf::Font;
        sf::Text *text = new sf::Text;

        if (!font->loadFromFile(data->path_to_resource)) {
            std::cerr << "Loading font failed!" << std::endl;
            return nullptr;
        }
        text->setFont(*font);
        text->setCharacterSize(24);
        text->setString(data->text);
        tmp->second = TEXT;
        tmp->third = text;
    }
    if (data->type == RECT)
    {
        tmp->second = RECT;
        tmp->third = new sf::RectangleShape;
    }
    _objects.emplace_back(tmp);
    return tmp->third;
}

void sfml::draw(void *object, int x0, int x1, int y0, int y1)
{
    sf::Vector2f pos(x0, y0);

    for (auto &tmp : _objects)
    {
        if (object == tmp->third)
        {
            if (tmp->second == SPRITE)
            {
                sf::Sprite *sp = static_cast<sf::Sprite *>(tmp->third);
                sp->setPosition(pos);
                sp->setScale(calcScale(*sp, x0, x1, y0, y1));
                _window.draw(*sp);
            }
            if (tmp->second == RECT)
            {
                sf::Vector2f vector(x1 - x0, y1 - y0);
                sf::RectangleShape rect(vector);

                rect.setPosition(pos);
                rect.setFillColor(getColor(tmp->first.color_name));
                _window.draw(rect);
            }
            if (tmp->second == TEXT)
            {
                sf::Text *t = static_cast<sf::Text *>(tmp->third);
                t->setPosition(pos);
                t->setFillColor(getColor(tmp->first.color_name));
                _window.draw(*t);
            }
        }
    }
}

void sfml::deleteObject(void *object)
{
    Triple<object_creation_data, object_type, void *>*data = static_cast<Triple<object_creation_data, object_type, void *>*>(object);
    for (size_t i = 0; i < _objects.size(); i += 1) {
        if (_objects[i] == data) {
            if (_objects[i]->second == TEXT)
            {
                sf::Text *tmp = static_cast<sf::Text *>(_objects[i]->third);
                delete tmp;
            }
            if (_objects[i]->second == RECT)
            {
                sf::RectangleShape *tmp = static_cast<sf::RectangleShape *>
                    (_objects[i]->third);
                delete tmp;
            }

            if (_objects[i]->second == TEXT)
            {
                sf::Sprite *tmp = static_cast<sf::Sprite *>(_objects[i]->third);
                delete tmp;
            }
            _objects.erase(_objects.begin() + i);
            break;
        }
    }
}

std::string sfml::getPressedKey(void)
{
    sf::Event ev;

    if (_window.pollEvent(ev)) {
        switch (ev.type) {
            case sf::Event::Closed:
                return "exit";
            case sf::Event::KeyPressed:
                return getKey(ev);
            default:
                return "";
        }
    }
    return "";
}

void sfml::destroy_screen()
{
    this->_window.close();
}

void sfml::clear_screen()
{
    this->_window.clear();
}

void sfml::display_screen()
{
    this->_window.display();
}

std::string sfml::getKey(sf::Event ev)
{
    sf::Keyboard::Key key;

    for (int i = sf::Keyboard::A; i != sf::Keyboard::Z; i += 1) {
        key = static_cast<sf::Keyboard::Key>(i);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
            return "prev_gr";
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
            return "next_gr";
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
            return "prev_game";
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
            return "next_game";
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            return "exit";
        if (sf::Keyboard::isKeyPressed(key))
            return std::string(1, i + 97);
    }
    for (int i = sf::Keyboard::Num0; i != sf::Keyboard::Num9; i += 1) {
        key = static_cast<sf::Keyboard::Key>(i);
        if (sf::Keyboard::isKeyPressed(key))
            return std::string(1, i + 22);
    }
    switch (ev.key.code) {
        case sf::Keyboard::Escape:
            return "esc";
        case sf::Keyboard::Enter:
            return "enter";
        case sf::Keyboard::Space:
            return "space";
        case sf::Keyboard::BackSpace:
            return "backspace";
        case sf::Keyboard::Tab:
            return "tab";
        case sf::Keyboard::Up:
            return "up";
        case sf::Keyboard::Left:
            return "left";
        case sf::Keyboard::Right:
            return "right";
        case sf::Keyboard::Down:
            return "down";
        default:
            return "*";
    }
}

sf::Vector2f
sfml::calcScale(const sf::Sprite &sprite, int x0, int x1, int y0, int y1)
{
    sf::Texture const *texture = sprite.getTexture();
    sf::Vector2u textureSize = texture->getSize();
    sf::Vector2f spriteSize = {static_cast<float>(x1 - x0), static_cast<float>(y1 - y0)};
    sf::Vector2f scale = {static_cast<float>(spriteSize.x / textureSize.x),
                          static_cast<float>(spriteSize.y / textureSize.y)};

    return scale;
}

sf::Color sfml::getColor(std::string colorcode)
{
    if (colorcode == "red")
        return sf::Color::Red;
    else if (colorcode == "green")
        return sf::Color::Green;
    else if (colorcode == "yellow")
        return sf::Color::Yellow;
    else if (colorcode == "blue")
        return sf::Color::Blue;
    else if (colorcode == "magenta")
        return sf::Color::Magenta;
    else if (colorcode == "cyan")
        return sf::Color::Cyan;
    else if (colorcode == "black")
        return sf::Color::Black;
    else
        return  sf::Color::White;
}

sfml::~sfml()
{
    for (auto &del : _objects)
    {
        if (del->second == TEXT)
        {
            sf::Text *tmp_del = static_cast<sf::Text *>(del->third);
            delete tmp_del;
        }
        if (del->second == SPRITE)
        {
            sf::Sprite *tmp_del = static_cast<sf::Sprite *>(del->third);
            delete tmp_del;
        }
        if (del->second == RECT)
        {
            sf::RectangleShape *tmp_del = static_cast<sf::RectangleShape *>
            (del->third);
            delete tmp_del;
        }
        delete del;
    }
}
