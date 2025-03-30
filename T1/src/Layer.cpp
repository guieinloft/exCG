#include "gl_canvas2d.h"
#include "Image.h"
#include "Layer.h"

Layer::Layer(int x, int y) {
    this->x = x;
    this->y = y;
    this->w = 0;
    this->h = 0;
    this->visible = true;
    this->opacity = 255;
    this->image = new Image();
}

void Layer::loadImage(char *path) {
    this->image->bmp_load(path);
    this->w = image->get_w();
    this->h = image->get_h();
}

void Layer::createBlank(int w, int h) {
    this->image->clear_image(w, h);
    this->w = w;
    this->h = h;
}

void Layer::Render(int x, int y) {
    this->image->render(this->x + x, this->y + y, this->opacity);
}

int Layer::get_x() {
    return x;
}

int Layer::get_y() {
    return y;
}

Image *Layer::getImage() {
    return this->image;
}
