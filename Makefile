.PHONY: $(wildcard *.cpp)

all: $(patsubst %.cpp, %.out, $(wildcard *.cpp))

%.out: %.cpp
	g++ $< -ljsoncpp -o $@ && ./$@ | node papaya.js
