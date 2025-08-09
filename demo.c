#include "Berry2D.h"

static SpriteSheet *test;
static SpriteSheet *font;

static int x = 30, y = 30;

void init() {

	test = load_sprite_sheet("test.png", 16, 16, 2);
    test = load_sprite_sheet("font.png", 6, 6, 26);
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
}