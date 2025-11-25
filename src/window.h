/*
 * window.c handles all game rendering, input tracking, and object management;
 * player.c handles player logic agnostic to the above
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



// player stuff
typedef struct {

	float  x,  y;
	float dx, dy;

	bool flipped;
	bool crouched;
	bool has_charge_power;

	int sprite_index;

} Player;

void process_player(unsigned long time, Input *input, Player *player, SpriteMap *level);

#endif