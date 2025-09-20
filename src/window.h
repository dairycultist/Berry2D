#ifndef WINDOW_H
#define WINDOW_H

#define TRUE 1
#define FALSE 0

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

#define UP 15       // arrow keys
#define DOWN 13
#define LEFT 11
#define RIGHT 9
#define CONFIRM 7   // Z
#define CANCEL 5    // X
#define MENU 3      // C

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

// rendering function prototypes
void set_clear_color(unsigned char r, unsigned char g, unsigned char b);

Sprite *load_sprite(const char *path);
void draw_sprite(Sprite *sprite, int x, int y);
void free_sprite(Sprite *sprite);

SpriteSheet *load_sprite_sheet(const char *path, int sprite_width, int sprite_height);
void draw_sprite_from_sheet(SpriteSheet *sprite_sheet, int index, int x, int y);
void draw_grid(SpriteSheet *sprite_sheet, int *indices, int indices_width, int indices_height, int x, int y);
void draw_text(SpriteSheet *sprite_sheet, char *text, int x, int y);
void free_sprite_sheet(SpriteSheet *sprite_sheet);

// helper for converting a grid of (essentially boolean) ints to a grid of properly-connected tiles following the assumed format
// all assume tile indices correspond to a properly formatted 8x8 tilemap (format is the same for all three, but will vary in how much of it is filled in)
void convert_indices_to_9_tile(int *indices, int indices_width, int indices_height);
// void convert_indices_to_16_tile(int *indices, int indices_width, int indices_height);
// void convert_indices_to_47_tile(int *indices, int indices_width, int indices_height);

#endif