#include "window.h"
#include "logic.h"

static SpriteSheet *player_sprite;
static SpriteSheet *grid_sprites;
static SpriteSheet *font;

static int camera_x = 0;

static float player_x = 50, player_y = 50;
static float player_dx = 0, player_dy = 0;

static bool flipped = FALSE;
static float run_cycle_timer;

static unsigned long time_of_last_jump;
static unsigned long time_of_last_pressed_jump;
static unsigned long time_of_last_grounded;

#define MAX_RUN_SPEED 2.5
#define SLIPPERINESS 0.97

#define LEVEL_WIDTH 400
#define LEVEL_HEIGHT 13

#define ABS(x) ((x) > 0 ? (x) : -(x))

static int sprite_indices[LEVEL_WIDTH * LEVEL_HEIGHT]; // TODO tile type grid that generates multiple tile (texture) indices grids (one for each type)

void init() {

	player_sprite = load_sprite_sheet("res/char.png", 16, 32);
	grid_sprites = load_sprite_sheet("res/tiles.png", 16, 16);
    font = load_sprite_sheet("res/font.png", 6, 7);

	for (int x = 0; x < LEVEL_WIDTH; x++) {
		for (int y = 7; y < LEVEL_HEIGHT; y++) {

			sprite_indices[x + y * LEVEL_WIDTH] = y > 9 ? 1 : 1 - ((x / 3) % 2);
		}
	}

	connect_indices(sprite_indices, LEVEL_WIDTH, LEVEL_HEIGHT);

	set_clear_color(65, 65, 120);
}

static int aabb_collides(int w, int h, int x, int y) {

	w--;
	h--;

	if (x < 0 || x + w >= LEVEL_WIDTH * 16)
		return 1;

	if (y < 0 || y + h >= LEVEL_HEIGHT * 16)
		return 0;

	return sprite_indices[x / 16 + y / 16 * LEVEL_WIDTH]
		|| sprite_indices[(x + w) / 16 + y / 16 * LEVEL_WIDTH]
		|| sprite_indices[x / 16 + (y + h) / 16 * LEVEL_WIDTH]
		|| sprite_indices[(x + w) / 16 + (y + h) / 16 * LEVEL_WIDTH];
}

void process(unsigned long time, int input) {

    if (JUST_PRESSED(UP, input)) {

		// register a jump at this time
		time_of_last_pressed_jump = time;
	}

	if (aabb_collides(16, 32, player_x, player_y + 1)) {

		// register grounded at this time
		time_of_last_grounded = time;
	}

	// if recently pressed jump (input caching) AND recently was on ground (coyote time), jump
	if (time - time_of_last_pressed_jump < 10 && time - time_of_last_grounded < 10) {

		player_dy = -3.0;
		time_of_last_jump = time;
		time_of_last_grounded = 0;
	}

	// gravity (if recently jumped, make gravity lesser if holding jump, otherwise greater)
	// ("recently jumped" is longer the faster you're running)
	if ((time - time_of_last_jump) < 12 + ABS(player_dx) * 2) {

		if (PRESSED(UP, input)) {
			player_dy += 0.04;
		} else {
			player_dy += 0.25;
		}
	} else {
		player_dy += 0.15;
	}

	// running
    if (PRESSED(LEFT, input) && !PRESSED(RIGHT, input)) {

        player_dx = player_dx * SLIPPERINESS - MAX_RUN_SPEED * (1.0 - SLIPPERINESS);
		flipped = TRUE;

	} else if (PRESSED(RIGHT, input) && !PRESSED(LEFT, input)) {

        player_dx = player_dx * SLIPPERINESS + MAX_RUN_SPEED * (1.0 - SLIPPERINESS);
		flipped = FALSE;

	} else
		player_dx *= SLIPPERINESS;

	// move player w/ collision
	while (aabb_collides(16, 32, player_x + player_dx, player_y)) {
		player_dx *= 0.7;
	}
	player_x += player_dx;

	while (aabb_collides(16, 32, player_x, player_y + player_dy)) {
		player_dy *= 0.7;
	}
	player_y += player_dy;

	// update camera
	while (WIDTH / 2 - ((int) player_x + 8) + camera_x <= 0) {
		camera_x++;
	}

	// render
	draw_grid(grid_sprites, sprite_indices, LEVEL_WIDTH, LEVEL_HEIGHT, -camera_x, 0);

	int player_sprite_index = 0;

	if (ABS(player_dx) < 0.3) {
		run_cycle_timer = 0.0;
		player_sprite_index = 0;
	} else {
		run_cycle_timer += (ABS(player_dx) + 1) / 20;
		
		if (run_cycle_timer >= 4.0) {
			run_cycle_timer -= 4.0;
		}

		player_sprite_index = run_cycle_timer < 3.0 ? 1 + (int) run_cycle_timer : 2;
	}

	draw_sprite_from_sheet(
		player_sprite,
		player_sprite_index,
		(int) player_x - camera_x,
		(int) player_y,
		flipped
	);

    draw_text(font, "ARROW KEYS TO MOVE\nZ IS CONFIRM\nX IS CANCEL\nC IS MENU", 100, 20);
}