#ifndef CANVAS_H
#define CANVAS_H

#include "Layer.h"
#include "Image.h"

//ignora alpha, opacidade e blending entre camadas
//trata o alpha como se fosse transparencia binaria
#define FAST_RENDER 1

class Canvas {
    public:
    Canvas(int w, int h);
    void Render(int sw, int sh, Layer **layers, int n_layers);
    int get_x();
    int get_y();
    int get_w();
    int get_h();
    void update();

    private:
    int x, y;
    int w, h;
    bool up;
    Image *canvas_bg;
    Image *canvas;
};

#endif
