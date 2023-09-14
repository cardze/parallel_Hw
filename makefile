.PHONY: clean

all: directories bin/main

bin/main: src/main.cpp
	g++ -std=c++11 src/main.cpp -o bin/pi

directories:
	mkdir -p bin

clean:
	rm -f bin/*