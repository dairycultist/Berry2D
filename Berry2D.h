#ifndef BERRY2D
#define BERRY2D

// screen size
#define WIDTH 320
#define HEIGHT 200
#define ASPECT_RATIO (WIDTH / (float) HEIGHT)

// input bitwise operations
#define UP_JUST_CHANGED(input)  ((input) & (1 << 14))
#define UP_PRESSED(input)       ((input) & (1 << 15))
#define UP_JUST_PRESSED(input)  (UP_JUST_CHANGED(input) && UP_PRESSED(input))
#define UP_RELEASED(input)      (!UP_PRESSED(input))
#define UP_JUST_RELEASED(input) (UP_JUST_CHANGED(input) && !UP_PRESSED(input))

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