#include "Canvas.h"
#include "LayerList.h"
#include "gl_canvas2d.h"

Canvas::Canvas(int w, int h) {
    this->w = w;
    this->h = h;
    this->up = true;
    canvas_bg = new Image();
    canvas = new Image();
    canvas_bg->bmp_load("images/canvas_bg/canvas_bg.bmp");
    canvas->copy(canvas_bg);
}

Canvas::~Canvas() {
    //delete canvas_bg;
    //delete canvas;
}

void Canvas::Render(int sw, int sh, Layer **layers, int n_layers) {
    this->x = (sw - w)/2;
    this->y = (sh - h)/2;
    for (int i = 0; i < sh; i += 8) {
        for (int j = 0; j < sw; j += 8) {
            if ((i + j) & 8)
                CV::color(0.75, 0.75, 0.75);
            else
                CV::color(0.5, 0.5, 0.5);
            CV::rectFill(j, i, j+8, i+8);
        }
    }
#if FAST_RENDER == 0
    if (up) {
        canvas->copy(canvas_bg);
        for (int i = 0; i < n_layers; i++)
            if (layers[i]->getVisibility())
                canvas->blend(layers[i]->getImage(), layers[i]->get_x(),
                layers[i]->get_y(), 0, 0, layers[i]->getOpacity());
        up = false;
    }
    canvas->render(x, y, 255);
#else
    for (int i = 0; i < n_layers; i++)
        if (layers[i]->getVisibility())
            layers[i]->getImage()->render(this->x + layers[i]->get_x(),
                this->y + layers[i]->get_y(), layers[i]->getOpacity());
#endif
    CV::color(0.15, 0.15, 0.15);
    CV::rectFill(0, 0, x, sh);
    CV::rectFill(x + w, 0, sw, sh);
    CV::rectFill(0, 0, sw, y);
    CV::rectFill(0, y + h, sw, sh);
}

int Canvas::get_x() {
    return x;
}

int Canvas::get_y() {
    return y;
}

int Canvas::get_w() {
    return w;
}

int Canvas::get_h() {
    return h;
}

void Canvas::update() {
    up = true;
}
