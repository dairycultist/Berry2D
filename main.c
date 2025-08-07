#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// draw_sprite(x, y, texture_data, sprite_w, sprite_h)
// draw_text(string, x, y, texture_data, glyph_w, glyph_h)

#define WIDTH 320
#define HEIGHT 200



static SDL_Texture *texture;
static SDL_Rect texture_rect = {0, 0, 16, 16};

void init(SDL_Renderer *renderer) {

	texture = IMG_LoadTexture(renderer, "test.png");
}

void process(SDL_Renderer *renderer) { // will also contain booleans for input

	SDL_RenderCopy(renderer, texture, NULL, &texture_rect);
}



int main() {

	printf("Starting Berry2D\n");

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Error initializing SDL:\n%s\n", SDL_GetError());
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("Berry2D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);

	if (!window) {
		printf("Error creating window:\n%s\n", SDL_GetError());
		return 1;
    }

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) {
		printf("Error creating renderer:\n%s\n", SDL_GetError());
		return 1;
	}

	SDL_Texture *screen_buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);

	init(renderer);

	// process events until window is closed
	SDL_Event event;
	SDL_Rect letterbox = {0, 0, WIDTH, HEIGHT}; // TODO dynamically change based on screen resize
	char running = 1;

	while (running) {

		while (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {

				running = 0;
			}
		}

		SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255); 			// clear window to grey
		SDL_RenderClear(renderer);
		SDL_SetRenderTarget(renderer, screen_buffer); 				// set render target to screen_buffer
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 			// clear screen_buffer to black
		SDL_RenderClear(renderer);
		process(renderer); 											// render stuff to screen_buffer
		SDL_SetRenderTarget(renderer, NULL); 						// reset render target back to window
		SDL_RenderCopy(renderer, screen_buffer, NULL, &letterbox); 	// render screen_buffer
		SDL_RenderPresent(renderer); 								// present rendered content to screen

		SDL_Delay(1000 / 30);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}