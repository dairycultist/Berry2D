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

		SpriteSheet **sprite_sheets;
		int **sprite_indices;

		int map_width;
		int map_height;
		int *map; // indexes into sprite_sheets and sprite_indices to determine what sprite to place

} SpriteMap;

SpriteMap *load_sprite_map(SpriteSheet **sprite_sheets, int map_width, int map_height, int *map);
void draw_sprite_map(SpriteMap *map, int x, int y);
void free_sprite_map(SpriteMap *map);

// implemented in logic.c
void init();
void process(unsigned long time, int input);

#endif