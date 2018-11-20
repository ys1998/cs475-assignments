CC=g++

UNAME_S := $(shell uname -s)

# CC
ifeq ($(UNAME_S),Darwin)
  CC := clang++ -arch x86_64
  SHELL := bash
  RM := rm
  ECHO := /bin/echo
  CAT := cat
  PRINTF := printf
  SED := sed
  OPENGLLIB := -framework OpenGL
  LDFLAGS := 
  CPPFLAGS := -I/usr/local/include -I./ -I./components -I./characters
  CFLAGS := -std=c++11 -Wall -Wno-unused-function # -O3
else
  CC := g++
  SHELL := bash
  RM := rm
  ECHO := /bin/echo
  CAT := cat
  PRINTF := printf
  SED := sed
  OPENGLLIB := -lGL
  LDFLAGS := -L/usr/local/lib 
  CPPFLAGS := -I/usr/local/include -I./ -I./components -I./characters
  CFLAGS := -std=c++11 -O3 -Wall -Wno-unused-function
endif

GLEWLIB := -lGLEW
GLFWLIB := -lglfw
LIBS := $(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)

######################################

BIN := hierarchical_modelling
SRCS := $(wildcard *.cpp) $(wildcard components/*.cpp) $(wildcard characters/*.cpp)

INCLUDES := $(wildcard *.hpp) $(wildcard *.glsl) $(wildcard components/*.hpp) $(wildcard characters/*.hpp)

all: $(BIN)

$(BIN): $(SRCS) $(INCLUDES)
	g++ $(CPPFLAGS) $(SRCS) -o $(BIN) $(LDFLAGS) $(LIBS) $(CFLAGS)

clean:
	rm -f *~ *.o $(BIN)
