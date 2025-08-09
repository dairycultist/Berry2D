#include "Berry2D.h"

static SpriteSheet *test;
static SpriteSheet *font;

static int x = 30, y = 80;

static int grid[] = {
    0, 1, 2, 3,
    0, 2, 2, 2,
    1, 1, 0, 0,
    2, 3, 2, 3
};

void init() {

	test = load_sprite_sheet("test.png", 16, 16, 2);
    font = load_sprite_sheet("font.png", 6, 7, 26);
}

void process(unsigned long time, int input) {

    if (PRESSED(UP, input))
        y -= 1;

    if (PRESSED(DOWN, input))
        y += 1;

    if (PRESSED(LEFT, input))
        x -= 1;

    if (PRESSED(RIGHT, input))
        x += 1;

    draw_sprite_from_sheet(test, (time / 8) % 4, x, y);

    draw_grid(test, grid, 4, 4, 20, 20);
    draw_text(font, "YOU CAN DRAW TEXT\nINDIVIDUAL SPRITES\nSPRITES FROM SPRITE SHEETS THAT\nMAKE ANIMATION EASIER\nAND GRIDS OF SPRITES\n\nALSO USE WASD TO MOVE", 80, 20);
}