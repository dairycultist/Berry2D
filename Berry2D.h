// draw_sprite(x, y, texture_data, sprite_w, sprite_h)
// draw_text(string, x, y, texture_data, glyph_w, glyph_h)

typedef struct {

    void *sdl_texture;
    int w;
    int h;

} Sprite;

/*
 * Framework-side implementation
 */
Sprite *load_sprite(const char *path);
void draw_sprite(Sprite *sprite, int x, int y);

/*
 * Game-side implementation
 */
void init();
void process(); // will also contain booleans for input