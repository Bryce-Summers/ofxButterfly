INCLUDE := -I.

# detect os
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
CXX := g++
LIBS := -std=c++0x -lGL -lGLU -lSDL
CXXFLAGS := -Wall -Wextra -pedantic -g -ggdb
endif

ifeq ($(UNAME), Darwin)
CXX := clang++
LIBS := -framework OpenGL -framework GLUT `sdl-config --cflags --libs`
CXXFLAGS := -std=c++11 -Wall -Wextra -pedantic -g -ggdb
endif

SOURCE := $(shell find . -type f -name "*.cpp")

default: clean butterfly

butterfly: 
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(SOURCE) $(LIBS) -o butterfly

clean:
	$(shell find . -iname "*.[od]" -exec rm '{}' ';')
	$(shell rm -f butterfly)
