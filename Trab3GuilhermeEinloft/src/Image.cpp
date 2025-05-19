#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "gl_canvas2d.h"

#include "Image.h"

#define sign(a) ((a > 0) - (a < 0))
#define min(a, b) ((a > b) * b + (a <= b) * a)

inline int tmax(int a, int b) {
    return (a > b) * a + (a <= b) * b;
}

Image::Image() {
    w = 0;
    h = 0;
    img = NULL;
}

void Image::close_image() {
    if (this->img == NULL) return;
    free(this->img);
}

void Image::bmp_load(const char *path) {
    //open file
    FILE *file = fopen(path, "rb");
    if (file == NULL) return;
    
    this->close_image();

    int hsize;
    uint16_t bpp;
    uint32_t offset;
    //header size
    fseek(file, 0xe, SEEK_SET);
    fread(&hsize, sizeof(uint32_t), 1, file);

    //read x and y dimensions
    fseek(file, 0x12, SEEK_SET);
    fread(&this->w, sizeof(uint32_t), 1, file);
    fread(&this->h, sizeof(uint32_t), 1, file);
    //read bytes per pixel
    fseek(file, 0x1c, SEEK_SET);
    fread(&bpp, sizeof(uint32_t), 1, file);
    bpp /= 8;

    //read file offset
    fseek(file, 0xa, SEEK_SET);
    fread(&offset, sizeof(uint32_t), 1, file);

    //create and read data array
    this->img = (uint8_t*)malloc(sizeof(uint8_t) * this->w * this->h * 4);
    fseek(file, offset, SEEK_SET);
    int padding = (4 - this->w * bpp) % 4;
    for (int i = (int)this->h - 1; i > -1; i--) {
        for (int j = 0; j < (int)this->w; j++) {
            fread(&this->img[(i * this->w + j) * 4], sizeof(uint8_t), bpp, file);
            for (int k = bpp; k < 4; k++)
                this->img[(i * this->w + j) * 4 + k] = 255;
        }
        fseek(file, padding, SEEK_CUR);
    }

    //close file
    fclose(file);
}

void Image::render(float x, float y, int o) {
    for (int i = 0; i < (int)(this->w * this->h); i++) {
        int base = 4 * i;
        if (this->img[base+3]) {
            CV::color(this->img[base+2]/255.0, this->img[base+1]/255.0 ,this->img[base]/255.0, this->img[base+3] * o /255.0/255.0);
            CV::point(x + i % this->w, y + i / this->w);
        }
    }
}

int Image::get_w() {
    if (this->img == NULL) return 0;
    return this->w;
}
int Image::get_h() {
    if (this->img == NULL) return 0;
    return this->h;
}

void Image::copy(Image *src) {
    this->close_image();
    this->w = src->w;
    this->h = src->h;
    this->img = (uint8_t*)malloc(sizeof(uint8_t) * src->w * src->h * 4);
    memcpy(this->img, src->img, sizeof(uint8_t) * src->w * src->h * 4);
}
