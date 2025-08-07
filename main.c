#include <SDL2/SDL.h>

// gcc -o build main.c $(sdl2-config --libs) $(sdl2-config --cflags)

#define WIDTH 320
#define HEIGHT 200

int main() {

	printf("Starting Berry2D\n");

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Could not initialize SDL\n");
		return 1;
	}

	// create the window
	SDL_Window *window = SDL_CreateWindow("Berry2D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 320, 200, SDL_WINDOW_RESIZABLE);

	if (!window) {
        printf("Could not create window");
		return 1;
    }

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	// process events until window is closed
	SDL_Event event;
	int running = 1;

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