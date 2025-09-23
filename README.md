# Berry2D (working title?)

A platformer engine akin to [Super Mario Bros. Remastered](https://github.com/JHDev2006/Super-Mario-Bros.-Remastered-Public).

- base game with player characters and levels
- modded player characters (allowing for stuff like [this](https://bsky.app/profile/honeyswells.bsky.social/post/3lz74rivjmk24))
- level editor which exports level file

320 × 200 (8:5) screen resolution, 60fps, D-pad + 3 action buttons

next big milestone is probably a level editor, tho I still gotta work on a level file format, enemies, ending a level, and a menu system as well.
I might add a "charge" mechanic where you break bricks when running into them fast enough as well, kinda like SMB3's P speed thingy, once you reach your max speed you'll have a different "charging" animation, also lets you ram through enemies instead of having to jump on them

## Installation

1. Berry2D relies on SDL2 and SDL2 Image. These can be installed __on Mac/Linux__ with the following commands:

```
brew install sdl2
brew install sdl2_image
```

2. Then, clone the repository.

```
git clone https://github.com/dairycultist/Berry2D
cd Berry2D
```

3. Finally, build and run the Berry2D executable `Berry2D/build`.

```
make run
```