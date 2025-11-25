#import <stdlib.h>
#import <string.h>

#include "window.h"

#define MAX_RUN_SPEED 2.5
#define MIN_CHARGE_SPEED 2.2
#define SLIPPERINESS 0.97
#define JUMP_SPEED -3.0

#define STANDING_HEIGHT 28
#define CROUCHING_HEIGHT 15

#define DEFAULT_GRAVITY 0.15
#define FAST_FALL_GRAVITY 0.35
#define HIGH_JUMP_GRAVITY 0.04

#define ABS(x) ((x) > 0 ? (x) : -(x))

static unsigned long time_of_last_pressed_jump = -100000; // game starts at time=0, if this starts at 0 then we will jump at game start
static unsigned long time_of_last_jump;
static unsigned long time_of_last_grounded;
static bool collided_horizontally;

static float run_cycle_timer;

static inline int point_collides(SpriteMap *level, int x, int y) {

	#define IS_SOLID(sheet) ((sheet) != 0 && (sheet) != 4)

	if (x < 0 || x >= level->map_width * 16)
		return 1;

	if (y < 0 || y >= level->map_height * 16)
		return 0;

	return IS_SOLID(level->sheet_map[x / 16 + y / 16 * level->map_width]);
}

static int aabb_collides(SpriteMap *level, int w, int h, int x, int y) {

	w--;
	h--;

	return point_collides(level, x, y) 			// top, left/right
		|| point_collides(level, x + w, y)
		|| point_collides(level, x, y + h / 2)  // middle, left/right (otherwise, you can slide between 1-tall terrain)
		|| point_collides(level, x + w, y + h / 2)
		|| point_collides(level, x, y + h) 		// bottom, left/right
		|| point_collides(level, x + w, y + h);
}

void process_player(unsigned long time, Input *input, Player *player, SpriteMap *level) {

	/*
	 * static player state changes
	 */

	// TEMP toggle between having and not having charge power with cancel
	if (input->cancel && input->cancel_justchanged)
		player->has_charge_power = !player->has_charge_power;

	// crouch input
	if (input->down && input->down_justchanged) {
		player->crouched = TRUE;
	} else if (!input->down && !aabb_collides(level, 16, STANDING_HEIGHT, player->x, player->y)) {
		player->crouched = FALSE;
	}

	// calculate bounding box
	int player_aabb_yoff = player->crouched ? (STANDING_HEIGHT - CROUCHING_HEIGHT) : 0;
	int player_aabb_h    = player->crouched ? CROUCHING_HEIGHT : STANDING_HEIGHT;

	if (input->up && input->up_justchanged) {

		// register a jump at this time
		time_of_last_pressed_jump = time;
	}

	if (aabb_collides(level, 16, player_aabb_h, player->x, player->y + player_aabb_yoff + 1)) {

		// register grounded at this time
		time_of_last_grounded = time;
	}

	/*
	 * update player velocity
	 */

	// if recently pressed jump (input caching) AND recently was on ground (coyote time), jump
	if (time - time_of_last_pressed_jump < 10 && time - time_of_last_grounded < 10) {

		player->dy = JUMP_SPEED;
		time_of_last_jump = time;
		time_of_last_grounded = 0;
	}

	// gravity (if recently jumped, make gravity lesser if holding jump, otherwise greater)
	// ("recently jumped" is longer the faster you're running)
	// TODO stop hardcoding these values
	if ((time - time_of_last_jump) < 10 + ABS(player->dx) * 4) {

		player->dy += input->up ? HIGH_JUMP_GRAVITY : FAST_FALL_GRAVITY;

	} else {

		player->dy += input->down ? FAST_FALL_GRAVITY : DEFAULT_GRAVITY;
	}

	if (player->crouched) {

		// crouch jumping
		if (input->left ^ input->right) {

			if (time_of_last_grounded != time)
				player->dx = player->dx * SLIPPERINESS + MAX_RUN_SPEED * (1.0 - SLIPPERINESS) * (input->left ? -1 : 1);
			player->flipped = input->left;
		}

		player->dx *= SLIPPERINESS; // punish the player for crouch jumping by making it always drag, even when inputing >:)

	} else {

		// running animation
		run_cycle_timer += (ABS(player->dx) + 1) / 20;
	
		if (run_cycle_timer >= 4.0) {
			run_cycle_timer -= 4.0;
		}

		// running
		if (input->left ^ input->right) {

			// update velocity
			player->dx = player->dx * SLIPPERINESS + MAX_RUN_SPEED * (1.0 - SLIPPERINESS) * (input->left ? -1 : 1);
			player->flipped = input->left;

			// charge power (immediately start charging in direction you're inputing if not already, even when airborne!)
			if (player->has_charge_power && ABS(player->dx) < MIN_CHARGE_SPEED && input->confirm && input->confirm_justchanged) {

				player->dx = (MAX_RUN_SPEED + 0.5) * (input->left ? -1 : 1);
			}

		} else
			player->dx *= SLIPPERINESS;
	}

	/*
	 * apply player velocity
	 */

	// move player w/ collision
	collided_horizontally = FALSE;

	// charging while grounded (and not crouching); instead of stopping horizontally on wall collision, you bounce back + up
	if (!player->crouched && ABS(player->dx) > MIN_CHARGE_SPEED && aabb_collides(level, 16, STANDING_HEIGHT, player->x, player->y + 1.0)) {

		if (aabb_collides(level, 16, STANDING_HEIGHT, player->x + player->dx, player->y)) {

			int index1 = (int) (player->x / level->sprite_width + 0.5 + (player->dx > 0 ? 1 : -1)) + (int) (player->y / level->sprite_height + 0.5) * level->map_width;
			int index2 = index1 + level->map_width;

			if (level->sheet_map[index1] == 2 && level->sheet_map[index2] == 2) {

				// destroy destructable blocks (sheet index 2)
				level->sheet_map[index1] = 0;
				level->sheet_map[index2] = 0;

			} else {

				// bounce
				player->dx *= -1;
				player->dy -= 1.5;
			}
		}

	// prevent player from moving horizontally through a wall
	} else {

		while (aabb_collides(level, 16, player_aabb_h, player->x + player->dx, player->y + player_aabb_yoff)) {

			player->dx *= 0.7;
			collided_horizontally = TRUE;

			if (ABS(player->dx) < 0.1)
				player->dx = 0;
		}
	}
	player->x += player->dx;

	while (aabb_collides(level, 16, player_aabb_h, player->x, player->y + player_aabb_yoff + player->dy)) {

		player->dy *= 0.7;

		if (ABS(player->dy) < 0.1)
				player->dy = 0;
	}
	player->y += player->dy;

	/*
	 * determine animation frame
	 */
	if (player->crouched) {

		// crouching
		player->sprite_index = 7;

	} else if (time_of_last_grounded != time) {

		// airborne
		run_cycle_timer = 0.0;
		player->sprite_index = 4;

	} else if ((ABS(player->dx) < 0.2 && !input->left && !input->right) || collided_horizontally) {

		// not moving
		run_cycle_timer = 0.0;
		player->sprite_index = 0;

	} else {

		// running AND charging
		if (ABS(player->dx) > MIN_CHARGE_SPEED) {

			player->sprite_index = 5 + ((int) run_cycle_timer) % 2;

		// running
		} else {

			player->sprite_index = run_cycle_timer < 3.0 ? 1 + (int) run_cycle_timer : 2;
		}
	}

	if (player->has_charge_power)
		player->sprite_index += 8;
}