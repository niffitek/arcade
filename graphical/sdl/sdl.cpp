/*
** EPITECH PROJECT, 2019
** FILE_NAME
** File description:
** FILE DESCRIPTION
*/

#include "sdl.hpp"

extern "C" {
IGraphics *create_graphics() {
    return new sdl;
}
}


static SDL_Color black {
        .r = 0,
        .g = 0,
        .b = 0,
        .a = 0
};

static SDL_Color white {
    .r = 255,
    .g = 255,
    .b = 255,
    .a = 0
};

static SDL_Color red {
        .r = 255,
        .g = 0,
        .b = 0,
        .a = 0
};

static SDL_Color green {
        .r = 0,
        .g = 255,
        .b = 0,
        .a = 0
};

static SDL_Color yellow {
        .r = 255,
        .g = 255,
        .b = 0,
        .a = 0
};

static SDL_Color blue {
        .r = 0,
        .g = 0,
        .b = 255,
        .a = 0
};

static SDL_Color cyan {
        .r = 0,
        .g = 255,
        .b = 255,
        .a = 0
};

static SDL_Color magenta {
        .r = 255,
        .g = 0,
        .b = 255,
        .a = 0
};

void sdl::init_screen(int width, int height) {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    _window = SDL_CreateWindow\
    ("Arcade", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    _renderer = SDL_CreateRenderer(_window, -1, 0);
    clear_screen();
    display_screen();
    _colors = {
            {"black", black},
            {"white", white},
            {"red", red},
            {"green", green},
            {"yellow", yellow},
            {"blue", blue},
            {"cyan", cyan},
            {"magenta", magenta}
    };
}

void *sdl::createObject(object_creation_data *data) {
    Obj graphic = {};
    SDL_Color color = getColor(data);

    if (data->type == SPRITE || data->type == TEXT) {
        SDL_Surface *tmpSurface;

        if (data->type == SPRITE) {
            if ((tmpSurface = IMG_Load(data->path_to_resource.c_str())) ==
            nullptr) {
                std::cerr << SDL_GetError() << std::endl;
                return nullptr;
            }
        } else {
            TTF_Font *font;
            if ((font = TTF_OpenFont(data->path_to_resource.c_str(),
                                             44)) == nullptr) {
                std::cerr << SDL_GetError() << std::endl;
                return nullptr;
            }
            if ((tmpSurface = TTF_RenderText_Solid(font, data->text.c_str(),
            color)) == nullptr) {
                std::cerr << SDL_GetError() << std::endl;
                return nullptr;
            }
            TTF_CloseFont(font);
        }
        graphic.toRender = SDL_CreateTextureFromSurface(_renderer, tmpSurface);
        graphic.height = tmpSurface->h;
        graphic.width = tmpSurface->w;
        SDL_FreeSurface(tmpSurface);
    } else {
        graphic.toRender = new SDL_Rect;
        graphic.color = color;
    }
    graphic.type = data->type;
    _graphics.emplace_back(graphic);
    return graphic.toRender;
}

void sdl::draw(void *object, int x0, int x1, int y0, int y1) {
    SDL_Rect rect = {x0, y0, x1 - x0, y1 - y0};

    if (object == nullptr)
        return;
    for (auto graphic : _graphics) {
        if (graphic.toRender == object) {
            if (graphic.type == TEXT) {
                rect.w = graphic.width;
                rect.h = graphic.height;
                SDL_RenderCopy(_renderer, (SDL_Texture*)graphic.toRender, nullptr, &rect);
            } else if (graphic.type == SPRITE) {
                SDL_RenderCopy(_renderer, (SDL_Texture*)graphic.toRender, nullptr, &rect);
            } else {
                auto color = graphic.color;
                SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
                SDL_RenderFillRect(_renderer, &rect);
            }
            break;
        }
    }
}

void sdl::deleteObject(void *object) {
    for (size_t i = 0; i < _graphics.size(); i += 1) {
        if (_graphics[i].toRender == object) {
            if (_graphics[i].type != RECT)
                SDL_DestroyTexture((SDL_Texture*)_graphics[i].toRender);
            _graphics.erase(_graphics.begin() + i);
            break;
        }
    }
}

std::string sdl::getPressedKey() {
    while (SDL_PollEvent(&_event)) {
        if (_event.type == SDL_QUIT || \
        (_event.type == SDL_WINDOWEVENT && _event.window.event == SDL_WINDOWEVENT_CLOSE))
            return "exit";
        else if (_event.type == SDL_KEYDOWN) {
            switch (_event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    return "esc";
                case SDLK_RETURN:
                    return "enter";
                case SDLK_SPACE:
                    return "space";
                case SDLK_BACKSPACE:
                    return "backspace";
                case SDLK_TAB:
                    return "tab";
                case SDLK_UP:
                    return "up";
                case SDLK_LEFT:
                    return "left";
                case SDLK_RIGHT:
                    return "right";
                case SDLK_DOWN:
                    return "down";
                case 'o':
                    return "prev_gr";
                case 'p':
                    return "next_gr";
                case 'k':
                    return "prev_game";
                case 'l':
                    return "next_game";
                default:
                    if ((_event.key.keysym.sym >= '0' && _event.key.keysym.sym <= '9') || \
                        (_event.key.keysym.sym >= 'a' && _event.key.keysym.sym <= 'z'))
                        return std::string(1, _event.key.keysym.sym);
                    return "*";
            }
        }
    }
    return "";
}

void sdl::destroy_screen() {
    for (auto graphic : _graphics) {
        if (graphic.type != RECT)
            SDL_DestroyTexture((SDL_Texture*)graphic.toRender);
    }
    auto it2 = _graphics.begin();
    while (it2 != _graphics.end())
        it2 = _graphics.erase(it2);

    SDL_DestroyWindow(_window);
    TTF_Quit();
    SDL_Quit();
}

void sdl::clear_screen() {
    SDL_RenderClear(_renderer);
}

void sdl::display_screen() {
    SDL_RenderPresent(_renderer);
}

SDL_Color sdl::getColor(object_creation_data *data) {
    SDL_Color color = white;

    std::string colorName = data->color_name;
    auto it = std::find_if(_colors.begin(), _colors.end(), \
    [&colorName](const std::pair<std::string, SDL_Color> &pair) {
        return pair.first == colorName;
    });
    if (it != _colors.end())
        color = it->second;
    return color;
}
