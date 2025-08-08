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

Clone the repository, build a `.o` file from the framework, implement a script including \<Berry2D.h\>, then compile it all together

```
gcc -o build main.c testgame.c -lSDL2_image $(sdl2-config --libs) $(sdl2-config --cflags)
```