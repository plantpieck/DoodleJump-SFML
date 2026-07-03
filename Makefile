COMPILER = g++
FLAGS = -std=c++17 -Wall -Iinclude -I/opt/homebrew/include -I/usr/local/include
LIBS = -L/opt/homebrew/lib -L/usr/local/lib -lsfml-graphics -lsfml-window -lsfml-system

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
APP_NAME = doodle_jump

build: $(APP_NAME)

$(APP_NAME): $(OBJECTS)
	$(COMPILER) $(OBJECTS) -o $(APP_NAME) $(LIBS)

src/%.o: src/%.cpp
	$(COMPILER) $(FLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(APP_NAME)