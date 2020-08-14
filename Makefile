CXX=g++
OBJECTS = main.cpp raytracer.h timer.h
EXE = main
STD = -std=c++17
CXX_FLAGS = -g -Wall -Wextra -Wshadow -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wnull-dereference -Wdouble-promotion -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wuseless-cast -O3 -DNDEBUG

.PHONY: clean

all: $(OBJECTS)
	$(CXX) $(STD) $(CXX_FLAGS) -o $(EXE) main.cpp -lsfml-graphics -lsfml-window -lsfml-system -lpthread

clean:
	rm -rf *.o main
