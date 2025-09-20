#include "window.h"
#include "logic.h"

static SpriteSheet *grid_sprites;
static SpriteSheet *font;

static float player_x = 50, player_y = 50;
static float player_dx = 0, player_dy = 0;
static unsigned long time_of_last_jump;

#define MAX_RUN_SPEED 2.5
#define SLIPPERINESS 0.97

#define LEVEL_WIDTH 400
#define LEVEL_HEIGHT 13

static int sprite_indices[LEVEL_WIDTH * LEVEL_HEIGHT];

void init() {

	grid_sprites = load_sprite_sheet("res/tiles.png", 16, 16);
    font = load_sprite_sheet("res/font.png", 6, 7);

	for (int x = 0; x < LEVEL_WIDTH; x++) {
		for (int y = 7; y < LEVEL_HEIGHT; y++) {

			sprite_indices[x + y * LEVEL_WIDTH] = y > 10 ? 1 : 1 - ((x / 3) % 2);
		}
	}

	connect_indices(sprite_indices, LEVEL_WIDTH, LEVEL_HEIGHT);

	set_clear_color(25, 25, 80);
}

static int player_collides_when_at(int x, int y) {

	if (x < 0 || x >= LEVEL_WIDTH * 16 || y < 0 || y >= LEVEL_HEIGHT * 16)
		return 0;

	return sprite_indices[x / 16 + y / 16 * LEVEL_WIDTH]
		|| sprite_indices[(x + 15) / 16 + y / 16 * LEVEL_WIDTH]
		|| sprite_indices[x / 16 + (y + 15) / 16 * LEVEL_WIDTH]
		|| sprite_indices[(x + 15) / 16 + (y + 15) / 16 * LEVEL_WIDTH];
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

	// move player w/ collision
	while (player_collides_when_at(player_x + player_dx, player_y)) {
		player_dx /= 2;
	}
	player_x += player_dx;

	while (player_collides_when_at(player_x, player_y + player_dy)) {
		player_dy /= 2;
	}
	player_y += player_dy;

	// render
	draw_grid(grid_sprites, sprite_indices, LEVEL_WIDTH, LEVEL_HEIGHT, 0, 0);

	draw_sprite_from_sheet(grid_sprites, 32, (int) player_x, (int) player_y);

    draw_text(font, "ARROW KEYS TO MOVE\nZ IS CONFIRM\nX IS CANCEL\nC IS MENU", 100, 20);
}