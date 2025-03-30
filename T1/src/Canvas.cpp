#include "Canvas.h"
#include "gl_canvas2d.h"

Canvas::Canvas(int w, int h) {
    this->w = w;
    this->h = h;
    for (int i = 0; i < MAX_LAYERS; i++) {
        layers[i] = new Layer(0, 0);
    }
    layers[0]->loadImage("images/testimg.bmp");
    layers[1]->loadImage("images/gremio2.bmp");
    layers[2]->loadImage("images/mushroom.bmp");
    this->n_layers = MAX_LAYERS;
    this->active_layer = 0;
}

void Canvas::Render(int sw, int sh) {
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
    for (int i = 0; i < MAX_LAYERS; i++) {
        layers[i]->Render(x, y);
    }
    CV::color(0.15, 0.15, 0.15);
    CV::rectFill(0, 0, x, sh);
    CV::rectFill(x + w, 0, sw, sh);
    CV::rectFill(0, 0, sw, y);
    CV::rectFill(0, y + h, sw, sh);
}

Layer *Canvas::getActiveLayer() {
    return layers[active_layer];
}

void Canvas::switchActiveLayer(int n) {
    this->active_layer = n % MAX_LAYERS;
}

int Canvas::get_x() {
    return x;
}

int Canvas::get_y() {
    return y;
}
