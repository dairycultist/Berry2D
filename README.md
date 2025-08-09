# Berry2D

A berry good 2D game framework (C + SDL) which I will definitely make games with.

320 × 200 (4:3), 30fps, D-pad

the cool part is that the game logic and underlying graphics/input calls (handled by SDL) are completely separate, meaning you could rewrite the latter (i.e. for a different platform) while maintaining compatibility with the former

## Using Berry2D

### Prerequisites

Berry2D relies on SDL2 and SDL2 Image. These can be installed __on Mac/Linux__ with the following commands:

```
brew install sdl2
brew install sdl2_image
```

### Setting up your workspace

1. Clone the repository.

```
git clone https://github.com/dairycultist/Berry2D
cd Berry2D
```

2. Build a `.o` file from the framework (and optionally delete the source file).

```
gcc -c -o Berry2D.o Berry2D.c $(sdl2-config --cflags)
rm Berry2D.c
```

3. Implement a script including `Berry2D.h` (see `demo.c` for an example).

4. Compile your script(s) together with `Berry2D.o` and run the build (replace `[YOUR_SCRIPTS]` with `demo.c` to compile the demo).

```
gcc [YOUR_SCRIPTS] -o build Berry2D.o -lSDL2_image $(sdl2-config --libs)
./build
```