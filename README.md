# Berry2D (working title?)

I wanna cut back the scope to just a single, dedicated game (which admittedly would probably be very easily modifiable)

- "a couch co-op platformer game with decent movement and cute art would be so fun" (my own words bro; [look at this](https://fixvx.com/Hyohnoo/status/1993024148486926431?s=20))
- clean up code (remove attempt at scenes, ensure code separation from window/renderer/IO)
- load levels from files that are just a big grid of numbers

---

A platformer engine akin to [Super Mario Bros. Remastered](https://github.com/JHDev2006/Super-Mario-Bros.-Remastered-Public).

- base game with player characters and levels
- modded player characters (allowing for stuff like [this](https://bsky.app/profile/honeyswells.bsky.social/post/3lz74rivjmk24))
- level editor which exports level file

next big milestone is probably a level editor, tho I still gotta work on a level file format, arbitrary entity system (particles + enemies + charge trail + items), parallax system (maybe part of entity system? render order...), ending a level, and a menu system as well.

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
