#ifndef LAYER_H
#define LAYER_H

#include "Image.h"

#include <stdio.h>
#include <stdint.h>

class Layer {
    public:
    Layer(int x, int y);
    void loadImage(char *path);
    void createBlank(int w, int h);
    void Render(int x, int y);
    void saveFile(FILE *file);
    void loadFile(FILE *file);
    int get_x();
    int get_y();
    void set_x(int x);
    void set_y(int y);
    void setVisibility(bool v);
    bool getVisibility();
    void setOpacity(uint8_t opacity);
    uint8_t getOpacity();
    Image *getImage();

    private:
    int x, y;
    Image *image;
    bool visible;
    uint8_t opacity;
};

#endif
