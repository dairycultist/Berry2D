# Berry2D (working title?)

pivoting from a framework (no one, not even me, is gonna use it anyways) to a platformer engine, like [Super Mario Bros. Remastered](https://github.com/JHDev2006/Super-Mario-Bros.-Remastered-Public)

- base game with player characters and levels
- modded player characters (allowing for stuff like [this](https://bsky.app/profile/honeyswells.bsky.social/post/3lz74rivjmk24))
- level editor which exports level file

320 × 200 (4:3), 60fps, D-pad + 3 action buttons

## Using Berry2D

### Prerequisites

Berry2D relies on SDL2 and SDL2 Image. These can be installed __on Mac/Linux__ with the following commands:

```
brew install sdl2
brew install sdl2_image
```

### Install

```
git clone https://github.com/dairycultist/Berry2D
cd Berry2D
make
```

This should create a folder called `Berry2D` with an executable inside called `build` which runs the game.