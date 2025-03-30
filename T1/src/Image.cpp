#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "gl_canvas2d.h"

#include "Image.h"

#define sign(a) ((a > 0) - (a < 0))
#define min(a, b) ((a > b) * b + (a <= b) * a)
#define max(a, b) ((a < b) * b + (a >= b) * a)

Image::Image() {
    w = 0;
    h = 0;
    img = NULL;
}

void Image::close_image() {
    if (this->img == NULL) return;
    free(this->img);
}

void Image::bmp_load(char *path) {
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
    for (int i = this->h - 1; i > -1; i--) {
        for (int j = 0; j < this->w; j++) {
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
    for (int i = 0; i < this->w * this->h; i++) {
        int base = 4 * i;
        if (this->img[base+3]) {
            CV::color(this->img[base+2]/255.0, this->img[base+1]/255.0 ,this->img[base]/255.0, this->img[base+3] * o /255.0/255.0);
            CV::point(x + i % this->w, y + i / this->w);
        }
    }
}

void Image::render_scaled(float x, float y, int new_w, int new_h, int o) {
    for (int i = 0; i < new_h; i++) {
        for (int j = 0; j < new_w; j++) {
            int base = (i * this->h / new_h * this->w + j * this->w / new_w) * 4;
            if (this->img[base+3]) {
                CV::color(this->img[base+2]/255.0, this->img[base+1]/255.0 ,this->img[base]/255.0, this->img[base+3] * o /255.0/255.0);
                CV::point(x + j, y + i);
            }
        }
    }
}

void Image::flip_h() {
    uint8_t *temp = (uint8_t*)malloc(sizeof(uint8_t) * 4);
    for (int i = 0; i < this->h; i++) {
        for (int j = 0; j < this->w / 2; j++) {
            memcpy(temp, &this->img[(i * this->w + j) * 4], 4);
            memcpy(&this->img[(i * this->w + j) * 4], &this->img[(i * this->w + this->w - 1 - j) * 4], 4);
            memcpy(&this->img[(i * this->w + this->w - 1 - j) * 4], temp, 4);
        }
    }
    free(temp);
}

void Image::flip_v() {
    uint8_t *temp = (uint8_t*)malloc(sizeof(uint8_t) * this->w * 4);
    for (int i = 0; i < this->h / 2; i++) {
        memcpy(temp, &this->img[i * this->w * 4], this->w * 4);
        memcpy(&this->img[i * this->w * 4], &this->img[(this->h - 1 - i) * this->w * 4], this->w * 4);
        memcpy(&this->img[(this->h - 1 - i) * this->w * 4], temp, this->w * 4);
    }
    free(temp);
}

void Image::resize(int new_w, int new_h) {
    uint8_t *new_img = (uint8_t*)malloc(sizeof(uint8_t) * new_w * new_h * 4);
    for (int i = 0; i < new_h; i++) {
        for (int j = 0; j < new_w; j++) {
            memcpy(&new_img[(i * new_w + j) * 4], &this->img[(i * this->h / new_h * this->w + j * this->w / new_w) * 4], 4);
        }
    }
    free(this->img);
    this->img = new_img;
    this->w = new_w;
    this->h = new_h;
}

int Image::get_w() {
    if (this->img == NULL) return 0;
    return this->w;
}
int Image::get_h() {
    if (this->img == NULL) return 0;
    return this->h;
}

void Image::put_pixel(int x, int y, int r, int g, int b, int a, bool blend) {
    if (x >= 0 && x < this->w && y >= 0 && y < this->h) {
        int base = (y * this->w + x) * 4;
        this->img[base] = b * (a * blend + 255 * !blend)/255.0 + this->img[base] * (255 - a)/255.0 * blend;
        this->img[base+1] = g * (a * blend + 255 * !blend)/255.0 + this->img[base+1] * (255 - a)/255.0 * blend;
        this->img[base+2] = r * (a * blend + 255 * !blend)/255.0 + this->img[base+2] * (255 - a)/255.0 * blend;
        this->img[base+3] = a * !blend + 255 * (blend && (this->img[base+3] >= 255 - a)) + (this->img[base+3] + a) * (blend && (this->img[base+3] < 255 - a));
    }
}

void Image::paint_square(int x, int y, int d, int r, int g, int b, int a, bool blend) {
    int d2 = d/2;
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            put_pixel(x - j + d2, y - i + d2, r, g, b, a, blend);
        }
    }
}

/*
void Image::paint_circle(int x, int y, int diameter, int r, int g, int b, bool v) {
    int px = 0, py = diameter/2, p = -diameter/2;
    while (px < py) {
        if (p > 0) {
            py -= 1;
            p += 2 * (px - py) + 1;
            for (int i = -px + 1; i < px; i++) {
                put_pixel(this, x + i, y + py, r, g, b, v);
                put_pixel(this, x + i, y - py, r, g, b, v);
            }
        }
        else p += 2*px + 1;

        put_pixel(this, x + px, y + py, r, g, b, v);
        put_pixel(this, x + px, y - py, r, g, b, v);
        put_pixel(this, x - px, y + py, r, g, b, v);
        put_pixel(this, x - px, y - py, r, g, b, v);
        for (int i = 0; i <= py; i++) {
            put_pixel(this, x + i, y + px, r, g, b, v);
            put_pixel(this, x + i, y - px, r, g, b, v);
            put_pixel(this, x - i, y + px, r, g, b, v);
            put_pixel(this, x - i, y - px, r, g, b, v);
        }

        px++;
    }
}
*/

void Image::paint_circle(int x, int y, int d, int r, int g, int b, int a, bool blend) {
    int rad = d/2;
    for (int i = -rad; i <= rad; i++) {
        for (int j = -rad; j <= rad; j++) {
            if (i * i + j * j <= rad * rad + rad)
                put_pixel(x + j, y + i, r, g, b, a, blend);
        }
    }
}

void Image::blend(Image src, int x, int y) {
    for (int i = max(0, y); i < min(this->h, src.h + y); i++) {
        for (int j = max(0, x); j < min(this->w, src.w + x); j++) {
            int b1 = (i * this->w + j) * 4;
            int b2 = ((i - y) * src.w + (j - x)) * 4;
            this->img[b1] = src.img[b2] * src.img[b2+3]/255.0 + this->img[b1] * (255 - src.img[b2+3])/255.0;
            this->img[b1+1] = src.img[b2+1] * src.img[b2+3]/255.0 + this->img[b1+1] * (255 - src.img[b2+3])/255.0;
            this->img[b1+2] = src.img[b2+2] * src.img[b2+3]/255.0 + this->img[b1+2] * (255 - src.img[b2+3])/255.0;
            this->img[b1+3] = 255 * (this->img[b1+3] >= 255 - src.img[b2+3]) + (this->img[b1+3] + src.img[b2+3]) * (this->img[b1+3] < 255 - src.img[b2+3]);
        }
    }
}

void Image::clear_image(int new_w, int new_h) {
    this->close_image();
    
    this->w = new_w;
    this->h = new_h;
    this->img = (uint8_t*)malloc(sizeof(uint8_t) * this->w * this->h * 4);

    for (int i = 0; i < this->h; i++) {
        for (int j = 0; j < this->w; j++) {
            for (int k = 0; k < 4; k++) {
                this->img[(i * this->w + j) * 4 + k] = 0;
            }
        }
    }
}
