#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// gcc -o build main.c -lSDL2_image $(sdl2-config --libs) $(sdl2-config --cflags)

// draw_sprite(x, y, texture_data, sprite_w, sprite_h)
// draw_text(string, x, y, texture_data, glyph_w, glyph_h)

#define WIDTH 320
#define HEIGHT 200

int main() {

	printf("Starting Berry2D\n");

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Error initializing SDL:\n%s\n", SDL_GetError());
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("Berry2D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 320, 200, SDL_WINDOW_RESIZABLE);

	if (!window) {
		printf("Error creating window:\n%s\n", SDL_GetError());
		return 1;
    }

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) {
		printf("Error creating renderer:\n%s\n", SDL_GetError());
		return 1;
	}

	SDL_Texture *texture = IMG_LoadTexture(renderer, "test.png");

	// process events until window is closed
	SDL_Event event;
	char running = 1;

	while (running) {

		while (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {

				running = 0;
			}
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
		SDL_RenderDrawLine(renderer, 0, 0, 20, 20);

		// present to screen
		SDL_RenderPresent(renderer);

		SDL_Delay(1000 / 30);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}