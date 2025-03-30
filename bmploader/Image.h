#ifndef IMAGE__H
#define IMAGE__H

class Image {
    public:
    Image();
    void close_image();
    void bmp_load(char *path);
    void render(float x, float y, int o);
    void render_scaled(float x, float y, int new_w, int new_h, int o);
    void flip_h();
    void flip_v();
    void resize(int new_w, int new_h);
    int get_w();
    int get_h();
    void put_pixel(int x, int y, int r, int g, int b, int a, bool blend);
    void paint_square(int x, int y, int d, int r, int g, int b, int a, bool blend);
    void paint_circle(int x, int y, int d, int r, int g, int b, int a, bool blend);
    void clear_image(int new_w, int new_h);
    void blend(Image src);

    private:
    uint32_t w;
    uint32_t h;
    uint8_t *img;
    //void render_ssmall(float x, float y, float sx, float sy);
    //void render_slarge(float x, float y, float sx, float sy);
};

#endif
