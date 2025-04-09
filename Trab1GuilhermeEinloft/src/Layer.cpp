#include "gl_canvas2d.h"
#include "Image.h"
#include "Layer.h"

#include <stdint.h>

Layer::Layer(int x, int y) {
    this->x = x;
    this->y = y;
    this->visible = true;
    this->opacity = 255;
    this->image = new Image();
}

Layer::~Layer() {
    delete image;
}

void Layer::loadImage(char *path) {
    this->image->bmp_load(path);
}

void Layer::saveFile(FILE *file) {
    fwrite(&x, sizeof(int), 1, file);
    fwrite(&y, sizeof(int), 1, file);
    fwrite(&visible, sizeof(bool), 1, file);
    fwrite(&opacity, sizeof(uint8_t), 1, file);
    image->file_save(file);
}

void Layer::loadFile(FILE *file) {
    fread(&x, sizeof(int), 1, file);
    fread(&y, sizeof(int), 1, file);
    fread(&visible, sizeof(bool), 1, file);
    fread(&opacity, sizeof(uint8_t), 1, file);
    image->file_load(file);
}

void Layer::createBlank(int w, int h) {
    this->image->clear_image(w, h);
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

void Layer::set_x(int x) {
    this->x = x;
}

void Layer::set_y(int y) {
    this->y = y;
}

Image *Layer::getImage() {
    return this->image;
}

void Layer::setVisibility(bool v) {
    this->visible = v;
}

bool Layer::getVisibility() {
    return this->visible;
}

void Layer::setOpacity(uint8_t opacity) {
    this->opacity = opacity;
}

uint8_t Layer::getOpacity() {
    return this->opacity;
}
