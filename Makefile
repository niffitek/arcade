SRCSCORE = \
	core/main.cpp	\
	core/menu.cpp	\
	core/graphical.cpp

SRCNCURSES = \
	graphical/ncurses/ncurses.cpp

SRCSFML = \
	graphical/sfml/sfml.cpp

SRCSDL = \
    graphical/sdl/sdl.cpp

SRCNIBBLER = \
	games/nibbler/Nibbler.cpp
	
SRCSOLARFOX = \
	games/solarfox/Solarfox.cpp

INC_CORE = -I core/
INC_NCURSES = -I graphical/ -I graphical/ncurses/
INC_SFML = -I graphical/ -I graphical/sfml/
INC_SDL = -I graphical/ -I graphical/sdl/
INC_NIBBLER = -I games/ -I nibbler/
INC_SOLARFOX = -I games/ -I Solarfox/

CXX = g++
VALGRIND = -g3
CXXFLAGS += -W -Wall -Wextra -fPIC

NAMECORE = arcade
NAMENCURSES = lib/arcade_ncurses.so
NAMESFML = lib/arcade_sfml.so
NAMESDL = lib/arcade_sdl2.so
NAMENIBBLER = lib/arcade_nibbler.so
NAMESOLARFOX = lib/arcade_solarfox.so

OBJCORE = $(SRCSCORE:.cpp=.o)
OBJNCURSES = $(SRCNCURSES:.cpp=.o)
OBJSFML = $(SRCSFML:.cpp=.o)
OBJSDL = $(SRCSDL:.cpp=.o)
OBJNIBBLER = $(SRCNIBBLER:.cpp=.o)
OBJSOLARFOX = $(SRCSOLARFOX:.cpp=.o)

all: core ncurses sfml sdl nibbler solarfox

core: $(NAMECORE)

games: nibbler solarfox

graphicals: ncurses sfml sdl

ncurses: $(NAMENCURSES)

sfml: $(NAMESFML)

sdl: $(NAMESDL)

nibbler: $(NAMENIBBLER)

solarfox: $(NAMESOLARFOX)

$(NAMECORE): $(OBJCORE)
	$(CXX) -o $(NAMECORE) $(OBJCORE) $(INC_CORE) -ldl -g

$(NAMENCURSES): $(OBJNCURSES)
	$(CXX) -o $(NAMENCURSES) $(OBJNCURSES) $(INC_NCURSES) -lncurses -shared -g

$(NAMESFML): $(OBJSFML)
	$(CXX) -o $(NAMESFML) $(OBJSFML) $(INC_SFML) -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system -shared -g

$(NAMESDL): $(OBJSDL)
	$(CXX) -o $(NAMESDL) $(OBJSDL) $(INC_SDL) -lSDL2 -lSDL2_image -lSDL2_ttf -shared -g

$(NAMENIBBLER): $(OBJNIBBLER)
	$(CXX) -o $(NAMENIBBLER) $(OBJNIBBLER) $(INC_NIBBLER) -shared -g

$(NAMESOLARFOX): $(OBJSOLARFOX)
	$(CXX) -o $(NAMESOLARFOX) $(OBJSOLARFOX) $(INC_SOLARFOX) -shared -g

clean:
	$(RM) $(OBJCORE)
	$(RM) $(OBJNCURSES)
	$(RM) $(OBJSFML)
	$(RM) $(OBJSDL)
	$(RM) $(OBJNIBBLER)
	$(RM) $(OBJSOLARFOX)

fclean: clean
	$(RM) $(NAMECORE)
	$(RM) $(NAMENCURSES)
	$(RM) $(NAMESFML)
	$(RM) $(NAMESDL)
	$(RM) $(NAMENIBBLER)
	$(RM) $(NAMESOLARFOX)

re: fclean all

.PHONY: clean fclean all re