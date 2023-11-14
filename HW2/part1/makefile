.PHONY: clean

all: directories bin/main

bin/main: main.cpp
	g++ -std=c++11 main.cpp -o bin/pi

directories:
	mkdir -p bin

clean:
	rm -f bin/*