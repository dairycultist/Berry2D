/*
 * window is an abstraction layer that implements I/O (keyboard input, rendering, audio, etc) and will call
 * two externally implemented functions (init and process) as appropriate
 */

#ifndef WINDOW_H
#define WINDOW_H

typedef int bool;

#define TRUE 1
#define FALSE 0

// screen size
#define WIDTH 384
#define HEIGHT 288
#define ASPECT_RATIO (WIDTH / (float) HEIGHT)

typedef struct {

    int up, down, left, right;
    int up_justchanged, down_justchanged, left_justchanged, right_justchanged;

	int confirm, cancel, menu;
	int confirm_justchanged, cancel_justchanged, menu_justchanged;

} Input;

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

// defines sprite indexing behaviour in SpriteMap->sprite_map
typedef enum {

	SELF_CONNECTING, ALL_CONNECTING, SINGLE_SPRITE, X_MOD_8

} SpriteSheetType;

typedef struct {

	SpriteSheet **sprite_sheets; // all need to have sprites that are the same size, or else undefined behaviour!
	SpriteSheetType *sprite_sheet_types;
	int sprite_sheet_count;

	int sprite_width;
	int sprite_height;

	int map_width;
	int map_height;

	int *sheet_map; // stores what sprite_sheet to use at each position
	int *sprite_map; // stores what sprite within a sprite sheet to use (independent of sprite sheet)

} SpriteMap; // might rename to Grid or TileGrid since it shares a name with ->sprite_map

SpriteMap *create_sprite_map(int sprite_width, int sprite_height, int map_width, int map_height);
void add_sprite_sheet_to_sprite_map(SpriteMap *sprite_map, const char *sprite_sheet_path, SpriteSheetType sprite_sheet_type);
void draw_sprite_map(SpriteMap *sprite_map, int x, int y);
void flush_sprite_map(SpriteMap *sprite_map); // if you update ->sheet_map, you need to flush the changes so they're reflected in ->sprite_map
void flush_sprite_map_at(int x, int y, SpriteMap *sprite_map);
void free_sprite_map(SpriteMap *sprite_map);

// implemented in logic.c
void init();
void process(Input *input); // MAYBE add double delta_time

#endif