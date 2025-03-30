#ifndef CANVAS_H
#define CANVAS_H

#include "Layer.h"

#define MAX_LAYERS 3

class Canvas {
    public:
    Canvas(int w, int h);
    void Render(int sw, int sh);
    Layer *getActiveLayer();
    void switchActiveLayer(int n);
    int get_x();
    int get_y();

    private:
    int x, y;
    int w, h;
    Layer *layers[MAX_LAYERS];
    int n_layers;
    uint8_t active_layer;
};

#endif
