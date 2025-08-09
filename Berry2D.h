// this file will end up with all the comments needed to fully explain how to use
// Berry2D, but for now I'm lazy

#ifndef BERRY2D
#define BERRY2D

// screen size
#define WIDTH 320
#define HEIGHT 200
#define ASPECT_RATIO (WIDTH / (float) HEIGHT)

// input bitwise operations
#define JUST_CHANGED(key, input)    ((input) & (1 << (key - 1)))
#define PRESSED(key, input)         ((input) & (1 << key))
#define JUST_PRESSED(key, input)    (JUST_CHANGED(key, input) && PRESSED(key, input))
#define RELEASED(key, input)        (!PRESSED(key, input))
#define JUST_RELEASED(key, input)   (JUST_CHANGED(key, input) && !PRESSED(key, input))

#define UP 15
#define DOWN 13
#define LEFT 11
#define RIGHT 9

// rendering datatypes
typedef struct {

    void *sdl_texture;
    int w;
    int h;

} Sprite;

typedef struct {

    void *sdl_texture;
    int sprite_w;
    int sprite_h;
    int sprites_per_row;

} SpriteSheet;

#endif

/*
 * Framework-side implementation
 */
Sprite *load_sprite(const char *path);
void draw_sprite(Sprite *sprite, int x, int y);
void free_sprite(Sprite *sprite);

SpriteSheet *load_sprite_sheet(const char *path, int sprite_width, int sprite_height, int sprites_per_row);
void draw_sprite_from_sheet(SpriteSheet *sprite_sheet, int index, int x, int y);
// void draw_grid(SpriteSheet *sprite_sheet, int sprite_wide, int sprite_high, int *indices);
void draw_text(SpriteSheet *sprite_sheet, char *text, int x, int y); // text sprite sheets should follow a specific format
void free_sprite_sheet(SpriteSheet *sprite_sheet);

/*
 * Game-side implementation
 */
void init();
void process(unsigned long time, int input);