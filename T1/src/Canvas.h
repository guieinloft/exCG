#ifndef CANVAS_H
#define CANVAS_H

#include "Layer.h"


class Canvas {
    public:
    Canvas(int w, int h);
    void Render(int sw, int sh, Layer **layers, int n_layers);
    int get_x();
    int get_y();

    private:
    int x, y;
    int w, h;
};

#endif
