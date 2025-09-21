#ifndef WINDOW_H
#define WINDOW_H

typedef int bool;

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

// rendering datatypes and function prototypes
void set_clear_color(unsigned char r, unsigned char g, unsigned char b);

typedef struct {

    void *sdl_texture;
    int w;
    int h;

} Sprite;

Sprite *load_sprite(const char *path);
void draw_sprite(Sprite *sprite, int x, int y, bool flip);
void free_sprite(Sprite *sprite);

typedef struct {

    void *sdl_texture;
    int sprite_w;
    int sprite_h;
    int sprites_per_row;

} SpriteSheet;

SpriteSheet *load_sprite_sheet(const char *path, int sprite_width, int sprite_height);
void draw_sprite_from_sheet(SpriteSheet *sprite_sheet, int index, int x, int y, bool flip);
void draw_text(SpriteSheet *sprite_sheet, char *text, int x, int y);
void free_sprite_sheet(SpriteSheet *sprite_sheet);

typedef struct {

	int layer_count;
	int **layers; // stores what sprite within a sprite sheet to use (each array shares an index with the sprite sheet it indexes into)
	SpriteSheet **sprite_sheets; // sprite sheet for each layer (all need to have sprites that are the same size, or else undefined behaviour)

	int map_width;
	int map_height;
	int *map; // stores what layer (+ sprite sheet) to use at each position

} SpriteMap;

// 1. add sprite_width and sprite_height to create_sprite_map
// 2. add add_layer_to_sprite_map(const char *sprite_sheet_path)
SpriteMap *create_sprite_map(SpriteSheet **sprite_sheets, int layer_count, int map_width, int map_height);
void draw_sprite_map(SpriteMap *sprite_map, int x, int y);
void flush_sprite_map(SpriteMap *sprite_map); // if you update ->map, you need to flush the changes so they're reflected in the layers (which determine what's drawn)
void free_sprite_map(SpriteMap *sprite_map);
// maybe add a "set_in_sprite_map" that will automatically flush the relevant layers, and only around the set position, if you
// don't want to update ->map and then flush

// implemented in logic.c
void init();
void process(unsigned long time, int input);

#endif