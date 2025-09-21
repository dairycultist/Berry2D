#import <stdlib.h>

static SpriteSheet *player_sprite;
static SpriteSheet *font;

static int camera_x = 0;

static float player_x = 50, player_y = 50;
static float player_dx = 0, player_dy = 0;

static bool flipped = FALSE;
static float run_cycle_timer;

static unsigned long time_of_last_jump;
static unsigned long time_of_last_pressed_jump;
static unsigned long time_of_last_grounded;
static bool collided_horizontally;

#define MAX_RUN_SPEED 2.5
#define SLIPPERINESS 0.97

#define LEVEL_WIDTH 400
#define LEVEL_HEIGHT 13

#define ABS(x) ((x) > 0 ? (x) : -(x))

static SpriteMap *level;

void init_level() {

	// obvious stuff
	player_sprite = load_sprite_sheet("res/char.png", 16, 32);
    font = load_sprite_sheet("res/font.png", 6, 7);
	set_clear_color(100, 180, 255);

	// initialize SpriteMap representing the level
	int level_init_data[LEVEL_WIDTH * LEVEL_HEIGHT] = {};

	for (int x = 0; x < LEVEL_WIDTH; x++) {
		for (int y = 7; y < LEVEL_HEIGHT; y++) {

			level_init_data[x + y * LEVEL_WIDTH] = y > 10 ? 1 : ((x / 6 + 1) % 3 % 2);
		}
	}

	SpriteSheet **sprite_sheets = malloc(sizeof(SpriteSheet *));

	sprite_sheets[0] = load_sprite_sheet("res/tiles.png", 16, 16);

	level = load_sprite_map(sprite_sheets, 1, LEVEL_WIDTH, LEVEL_HEIGHT, level_init_data);
}

static int aabb_collides(int w, int h, int x, int y) {

	w--;
	h--;

	if (x < 0 || x + w >= level->map_width * 16)
		return 1;

	if (y < 0 || y + h >= level->map_height * 16)
		return 1; // 0

	return 0;

	// return level->map[x / 16 + y / 16 * level->map_width]
	// 	|| level->map[(x + w) / 16 + y / 16 * level->map_width]
	// 	|| level->map[x / 16 + (y + h) / 16 * level->map_width]
	// 	|| level->map[(x + w) / 16 + (y + h) / 16 * level->map_width];
}

void process_level(unsigned long time, int input) {

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
	if ((time - time_of_last_jump) < 10 + ABS(player_dx) * 4) {

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
	collided_horizontally = FALSE;

	while (aabb_collides(16, 32, player_x + player_dx, player_y)) {
		player_dx *= 0.7;
		collided_horizontally = TRUE;
	}
	player_x += player_dx;

	while (aabb_collides(16, 32, player_x, player_y + player_dy)) {
		player_dy *= 0.7;
	}
	player_y += player_dy;

	// update camera
	int player_middle_pos = WIDTH / 2 - ((int) player_x + 8);

	while (player_middle_pos + camera_x >= 20 && camera_x > 0) { camera_x--; }
	while (player_middle_pos + camera_x <= -20) { camera_x++; }
}

void draw_level(unsigned long time, int input) {

	// render level
	draw_sprite_map(level, -camera_x, 0);

	// render player (determine animation too)
	int player_sprite_index = 0;

	if (time_of_last_grounded != time) {

		run_cycle_timer = 0.0;
		player_sprite_index = 4;

	} else if ((ABS(player_dx) < 0.2 && !PRESSED(LEFT, input) && !PRESSED(RIGHT, input)) || collided_horizontally) {

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

	// draw text
    draw_text(font, "ARROW KEYS TO MOVE\nZ IS CONFIRM\nX IS CANCEL\nC IS MENU", 100, 20);
}