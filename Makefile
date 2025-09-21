.PHONY: run clean

build: src/*
	gcc -o build src/*.c -lSDL2_image $(shell sdl2-config --cflags) $(shell sdl2-config --libs)

run: build
	./build

clean:
	rm build