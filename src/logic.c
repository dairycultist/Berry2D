#include "window.h"
#include "logic.h"

static SpriteSheet *tiles;
static SpriteSheet *font;

static float player_x = 30, player_y = 80;
static float player_dx = 0, player_dy = 0;
static unsigned long time_of_last_jump;

#define MAX_RUN_SPEED 2.5
#define SLIPPERINESS 0.97

static int grid[] = {
    1, 2, 3, 0,
    9, 10, 11, 0,
    17, 18, 19, 0,
    0, 0, 0, 0
};

void init() {

	tiles = load_sprite_sheet("res/tiles.png", 16, 16);
    font = load_sprite_sheet("res/font.png", 6, 7);

	set_clear_color(25, 25, 80);
}

void process(unsigned long time, int input) {

	// jumping
    if (JUST_PRESSED(UP, input)) {

        player_dy = -3.0;
		time_of_last_jump = time;
	}

	// gravity (less if 1. holding jump and 2. recently jumped)
	player_dy += (time - time_of_last_jump) < 20 && PRESSED(UP, input) ? 0.04 : 0.08;

	// running
    if (PRESSED(LEFT, input) && !PRESSED(RIGHT, input))
        player_dx = player_dx * SLIPPERINESS - MAX_RUN_SPEED * (1.0 - SLIPPERINESS);
    else if (PRESSED(RIGHT, input) && !PRESSED(LEFT, input))
        player_dx = player_dx * SLIPPERINESS + MAX_RUN_SPEED * (1.0 - SLIPPERINESS);
	else
		player_dx *= SLIPPERINESS;

	// move player (TODO check for collision)
	player_x += player_dx;
	player_y += player_dy;

	draw_grid(tiles, grid, 4, 4, 0, 0);

	draw_sprite_from_sheet(tiles, 8, (int) player_x, (int) player_y);

    draw_text(font, "ARROW KEYS TO MOVE\nZ IS CONFIRM\nX IS CANCEL\nC IS MENU", 100, 20);
}