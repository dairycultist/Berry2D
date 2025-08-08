#ifndef BERRY2D
#define BERRY2D

// screen size
#define WIDTH 320
#define HEIGHT 200
#define ASPECT_RATIO (WIDTH / (float) HEIGHT)

// input bitwise operations
#define JUST_CHANGED(key, input)    ((input) & (1 << (key - 1)))
#define PRESSED(key, input)         ((input) & (1 << key))
#define JUST_PRESSED(key, input)    (JUST_CHANGED(key, input) && PRESSED(key, input))
#define RELEASED(key, input)        (!PRESSED(key, input))
#define JUST_RELEASED(key, input)   (JUST_CHANGED(key, input) && !PRESSED(key, input))

#define UP 15
#define DOWN 13
#define LEFT 11
#define RIGHT 9

// rendering datatypes
typedef struct {

    void *sdl_texture;
    int w;
    int h;

} Sprite;

#endif

/*
 * Framework-side implementation
 */
Sprite *load_sprite(const char *path);
void draw_sprite(Sprite *sprite, int x, int y);
void free_sprite(Sprite *sprite);

// draw_sprite(x, y, texture_data, sprite_w, sprite_h)
// draw_text(string, x, y, texture_data, glyph_w, glyph_h)

// Tileset *load_tileset(const char *path, int tiles_wide, int tiles_high)
// void draw_tileset(Tileset *tileset, int width, int height, char *tiles)

/*
 * Game-side implementation
 */
void init();
void process(unsigned long time, int input);