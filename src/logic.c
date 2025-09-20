#include "window.h"
#include "logic.h"

static SpriteSheet *tiles;
static SpriteSheet *font;

static int x = 30, y = 80;

static int grid[] = {
    0, 1, 2, 3,
    4, 5, 6, 7,
    8, 9, 10, 7,
    7, 7, 7, 7
};

void init() {

	tiles = load_sprite_sheet("res/tiles.png", 16, 16);
    font = load_sprite_sheet("res/font.png", 6, 7);

	set_clear_color(25, 25, 80);
}

void process(unsigned long time, int input) {

    if (PRESSED(UP, input))
        y -= PRESSED(CONFIRM, input) ? 2 : 1;

    if (PRESSED(DOWN, input))
        y += PRESSED(CONFIRM, input) ? 2 : 1;

    if (PRESSED(LEFT, input))
        x -= PRESSED(CONFIRM, input) ? 2 : 1;

    if (PRESSED(RIGHT, input))
        x += PRESSED(CONFIRM, input) ? 2 : 1;

	draw_grid(tiles, grid, 4, 4, x, y);

    draw_text(font, "ARROW KEYS TO MOVE\nZ IS CONFIRM\nX IS CANCEL\nC IS MENU", 100, 20);
}