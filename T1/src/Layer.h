#ifndef LAYER_H
#define LAYER_H

#include "Image.h"

class Layer {
    public:
    Layer(int x, int y);
    void loadImage(char *path);
    void createBlank(int w, int h);
    void Render(int x, int y);
    int get_x();
    int get_y();
    void set_x(int x);
    void set_y(int y);
    Image *getImage();

    private:
    int x, y;
    int w, h;
    Image *image;
    bool visible;
    uint8_t opacity;
};

#endif
