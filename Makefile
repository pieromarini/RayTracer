OBJECTS = main.cpp
STD = -std=c++17
MYFLAGS = -g -Wall -Wextra -Wshadow -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wnull-dereference -Wdouble-promotion -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wuseless-cast
LINK = -lsfml-graphics -lsfml-window -lsfml-system
CXX = g++
EXEC = main

all: $(OBJECTS)
	$(CXX) $(STD) $(MYFLAGS) $(OBJECTS) -o $(EXEC) $(LINK)

clean: 
	rm $(EXEC)
