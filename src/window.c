#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "window.h"

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *screen_buffer;

static unsigned char clear_r, clear_g, clear_b;

int main() {

	printf("Starting Berry2D\n");

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Error initializing SDL:\n%s\n", SDL_GetError());
		return 1;
	}

	window = SDL_CreateWindow("Berry2D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH * 2, HEIGHT * 2, SDL_WINDOW_RESIZABLE);

	if (!window) {
		printf("Error creating window:\n%s\n", SDL_GetError());
		return 1;
    }

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) {
		printf("Error creating renderer:\n%s\n", SDL_GetError());
		return 1;
	}

	screen_buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);

	init();

	// process events until window is closed
	SDL_Event event;
	SDL_Rect letterbox = {0, 0, WIDTH * 2, HEIGHT * 2};

	char running = TRUE;

	unsigned long time = 0; // wraps to 0 at around 4.5 years

	int input = 0; // at least 16 bytes, aka 8 pairs of 'pressed?' and 'just changed?' flags (their positions are specified in window.h)

	while (running) {

		// clear 'just changed?' flag of every input
		input = input & 0b1010101010101010;

		while (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {

				running = FALSE;

			} else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {

				#define MIN(a, b) ((a) > (b) ? (b) : (a))

				// dynamically change letterbox based on screen resize
				letterbox.w = MIN(event.window.data1, event.window.data2 * ASPECT_RATIO);
				letterbox.h = MIN(event.window.data2, event.window.data1 / ASPECT_RATIO);

				letterbox.x = (event.window.data1 - letterbox.w) / 2;
				letterbox.y = (event.window.data2 - letterbox.h) / 2;

			} else if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && !event.key.repeat) {

				// first operation is changing 'pressed?' flag, second 'just changed?' flag (cleared on next frame)
				#define GET_INPUT_FROM_KEYSTATE(pressed, key) ((pressed ? input | (1 << key) : input & ~(1 << key)) | (1 << (key - 1)))

				switch (event.key.keysym.scancode) {
					case SDL_SCANCODE_UP:
						input = GET_INPUT_FROM_KEYSTATE(event.key.state == SDL_PRESSED, UP);
						break;
					case SDL_SCANCODE_DOWN:
						input = GET_INPUT_FROM_KEYSTATE(event.key.state == SDL_PRESSED, DOWN);
						break;
					case SDL_SCANCODE_LEFT:
						input = GET_INPUT_FROM_KEYSTATE(event.key.state == SDL_PRESSED, LEFT);
						break;
					case SDL_SCANCODE_RIGHT:
						input = GET_INPUT_FROM_KEYSTATE(event.key.state == SDL_PRESSED, RIGHT);
						break;
					case SDL_SCANCODE_Z:
						input = GET_INPUT_FROM_KEYSTATE(event.key.state == SDL_PRESSED, CONFIRM);
						break;
					case SDL_SCANCODE_X:
						input = GET_INPUT_FROM_KEYSTATE(event.key.state == SDL_PRESSED, CANCEL);
						break;
					case SDL_SCANCODE_C:
						input = GET_INPUT_FROM_KEYSTATE(event.key.state == SDL_PRESSED, MENU);
						break;
					default:
						break;
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255); 					// clear window to grey
		SDL_RenderClear(renderer);
		SDL_SetRenderTarget(renderer, screen_buffer); 						// set render target to screen_buffer
		SDL_SetRenderDrawColor(renderer, clear_r, clear_g, clear_b, 255); 	// clear screen_buffer to clear color (default black)
		SDL_RenderClear(renderer);
		process(time++, input); 											// let the programmer do logic/render stuff to screen_buffer
		SDL_SetRenderTarget(renderer, NULL); 								// reset render target back to window
		SDL_RenderCopy(renderer, screen_buffer, NULL, &letterbox); 			// render screen_buffer
		SDL_RenderPresent(renderer); 										// present rendered content to screen

		SDL_Delay(1000 / 60);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

/*
 * header function implementations
 */
void set_clear_color(unsigned char r, unsigned char g, unsigned char b) {
	
	clear_r = r;
	clear_g = g;
	clear_b = b;
}

Sprite *load_sprite(const char *path) {

	Sprite *sprite = malloc(sizeof(Sprite));

	sprite->sdl_texture = IMG_LoadTexture(renderer, path);

	SDL_QueryTexture(sprite->sdl_texture, NULL, NULL, &sprite->w, &sprite->h);

	return sprite;
}

void draw_sprite(Sprite *sprite, int x, int y, bool flip) {

	SDL_Rect texture_rect = { x, y, sprite->w, sprite->h };

	SDL_RenderCopyEx(renderer, sprite->sdl_texture, NULL, &texture_rect, 0.0, NULL, flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void free_sprite(Sprite *sprite) {
	
	SDL_DestroyTexture(sprite->sdl_texture);
	free(sprite);
}

SpriteSheet *load_sprite_sheet(const char *path, int sprite_width, int sprite_height) {

	SpriteSheet *sprite_sheet = malloc(sizeof(SpriteSheet));

	sprite_sheet->sdl_texture = IMG_LoadTexture(renderer, path);

	// get the width to determine the number of sprites per row (rounding down)
	int width;
	SDL_QueryTexture(sprite_sheet->sdl_texture, NULL, NULL, &width, NULL);

	sprite_sheet->sprite_w = sprite_width;
	sprite_sheet->sprite_h = sprite_height;
	sprite_sheet->sprites_per_row = width / sprite_width;

	return sprite_sheet;
}

void draw_sprite_from_sheet(SpriteSheet *sprite_sheet, int index, int x, int y, bool flip) {

	SDL_Rect copy_rect = {

		(index % sprite_sheet->sprites_per_row) * sprite_sheet->sprite_w,
		(index / sprite_sheet->sprites_per_row) * sprite_sheet->sprite_h,
		sprite_sheet->sprite_w,
		sprite_sheet->sprite_h
	};

	SDL_Rect paste_rect = { x, y, sprite_sheet->sprite_w, sprite_sheet->sprite_h };

	SDL_RenderCopyEx(renderer, sprite_sheet->sdl_texture, &copy_rect, &paste_rect, 0.0, NULL, flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

// text sprite sheets follows a specific format
void draw_text(SpriteSheet *sprite_sheet, char *text, int x, int y) {

	int start_x = x;

	while (*text) {

		if (*text >= 'A' && *text <= 'Z') {

			draw_sprite_from_sheet(sprite_sheet, *text - 65, x, y, FALSE);
			x += sprite_sheet->sprite_w;
		}

		else if (*text == '\n') {

			x = start_x;
			y += sprite_sheet->sprite_h;
		}

		else if (*text == ' ') {

			x += sprite_sheet->sprite_w;
		}

		text++;
	}
}

void free_sprite_sheet(SpriteSheet *sprite_sheet) {

	SDL_DestroyTexture(sprite_sheet->sdl_texture);
	free(sprite_sheet);
}

// helpers for converting a grid of (essentially boolean) ints to a grid of properly-connected tiles following the assumed 8x8 tilemap format
static int get_from_indexer(int *indexer, int indexer_width, int indexer_height, int x, int y) {

	if (x < 0 || x >= indexer_width || y < 0 || y >= indexer_height)
		return 0;

	return indexer[x + y * indexer_width];
}

static inline void smooth_at_point_in_indexer(int *indexer, int indexer_width, int indexer_height, int x, int y) {

	// 9, 16, 47 tile, currently 9
	int up, down, left, right;

	if (indexer[x + y * indexer_width] == 0)
		return;

	up = get_from_indexer(indexer, indexer_width, indexer_height, x, y - 1);
	down = get_from_indexer(indexer, indexer_width, indexer_height, x, y + 1);
	left = get_from_indexer(indexer, indexer_width, indexer_height, x - 1, y);
	right = get_from_indexer(indexer, indexer_width, indexer_height, x + 1, y);

	if (!up) {
		if (!left) {
			indexer[x + y * indexer_width] = 9;
		} else if (!right) {
			indexer[x + y * indexer_width] = 11;
		} else {
			indexer[x + y * indexer_width] = 10;
		}
	} else if (!down) {
		if (!left) {
			indexer[x + y * indexer_width] = 25;
		} else if (!right) {
			indexer[x + y * indexer_width] = 27;
		} else {
			indexer[x + y * indexer_width] = 26;
		}
	} else {
		if (!left) {
			indexer[x + y * indexer_width] = 17;
		} else if (!right) {
			indexer[x + y * indexer_width] = 19;
		} else {
			indexer[x + y * indexer_width] = 18;
		}
	}
}

void flush_sprite_map(SpriteMap *sprite_map) {

	for (int layer_index = 0; layer_index < sprite_map->layer_count; layer_index++) {

		// modify the sprite indexer to have TRUE only for values in map that match layer_index
		for (int i = 0; i < sprite_map->map_width * sprite_map->map_height; i++) {

			sprite_map->layers[layer_index][i] = sprite_map->map[i] == layer_index + 1;
		}

		// modify the sprite indexer to go from binary TRUE/FALSE to properly indexing the sprite sheet to smoothly connect
		for (int x = 0; x < sprite_map->map_width; x++) {
			for (int y = 0; y < sprite_map->map_height; y++) {

				smooth_at_point_in_indexer(sprite_map->layers[layer_index], sprite_map->map_width, sprite_map->map_height, x, y);
			}
		}
	}
}

SpriteMap *create_sprite_map(SpriteSheet **sprite_sheets, int layer_count, int map_width, int map_height) {

	SpriteMap *sprite_map = malloc(sizeof(SpriteMap));

	sprite_map->layer_count = layer_count;
	sprite_map->layers = malloc(sizeof(int *) * layer_count);
	sprite_map->sprite_sheets = sprite_sheets;

	sprite_map->map_width = map_width;
	sprite_map->map_height = map_height;
	sprite_map->map = calloc(map_width * map_height, sizeof(int));

	for (int layer_index = 0; layer_index < layer_count; layer_index++) {

		// create a layer the size of the map
		sprite_map->layers[layer_index] = malloc(sizeof(int) * map_width * map_height);
	}

	return sprite_map;
}

void draw_sprite_map(SpriteMap *sprite_map, int x, int y) {

	// calculate bounds of indices array that will actually be on screen
	// this is unreadable but it works ok
	int i_start = x >= 0 ? 0 : -x / sprite_map->sprite_sheets[0]->sprite_w;
	int j_start = y >= 0 ? 0 : -y / sprite_map->sprite_sheets[0]->sprite_h;

	int i_end = WIDTH >= (sprite_map->map_width + 1) * sprite_map->sprite_sheets[0]->sprite_w + x ? sprite_map->map_width : sprite_map->map_width + (WIDTH - sprite_map->map_width * sprite_map->sprite_sheets[0]->sprite_w - x) / sprite_map->sprite_sheets[0]->sprite_w;
	int j_end = HEIGHT >= (sprite_map->map_height + 1) * sprite_map->sprite_sheets[0]->sprite_h + y ? sprite_map->map_height : sprite_map->map_height + (HEIGHT - sprite_map->map_height * sprite_map->sprite_sheets[0]->sprite_h - y) / sprite_map->sprite_sheets[0]->sprite_h;

	// draw
	for (int i = i_start; i < i_end; i++) {
		for (int j = j_start; j < j_end; j++) {

			int layer_index = sprite_map->map[i + j * sprite_map->map_width];

			if (layer_index == 0)
				continue;

			layer_index--; // since layers are 1-indexed (0 is no render)
		
			draw_sprite_from_sheet(
				sprite_map->sprite_sheets[layer_index],
				sprite_map->layers[layer_index][i + j * sprite_map->map_width],
				x + i * sprite_map->sprite_sheets[layer_index]->sprite_w,
				y + j * sprite_map->sprite_sheets[layer_index]->sprite_h,
				FALSE
			);
		}
	}
}

void free_sprite_map(SpriteMap *sprite_map) {

	// TODO doesn't free attached sprite_sheets

	// free map
	free(sprite_map->map);

	// free all layers + layer array
	for (int i = 0; i < sprite_map->layer_count; i++)
		free(sprite_map->layers[i]);

	free(sprite_map->layers);

	// free the spritemap
	free(sprite_map);
}