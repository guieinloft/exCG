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

void rgb_to_ycc(uint8_t *img, int *y, int *cb, int *cr, int w, int h) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            //numeros retirados do padrão jpeg
            y[(i * w + j) + 0] = 
                0.299 * img[4 * (i * w + j) + 2]
                + 0.587 * img[4 * (i * w + j) + 1]
                + 0.114 * img[4 * (i * w + j) + 0];
            cb[(i * w + j) + 0] = round( 128
                - 0.168736 * img[4 * (i * w + j) + 2]
                - 0.331264 * img[4 * (i * w + j) + 1]
                + 0.500000 * img[4 * (i * w + j) + 0] );
            cr[(i * w + j) + 0] = round( 128
                + 0.500000 * img[4 * (i * w + j) + 2]
                - 0.418688 * img[4 * (i * w + j) + 1]
                - 0.081312 * img[4 * (i * w + j) + 0] );
        }
    }
}

uint8_t clamp (int a) {
    if (a < 0) return 0;
    if (a > 255) return 255;
    return a;
}

void ycc_to_rgb(uint8_t *img, int *y, int *cb, int *cr, int w, int h) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            //numeros retirados do padrão jpeg
            img[4 * (i * w + j) + 2] = clamp(round(
                y[i * w + j]
                + 1.402 * (cr[(i * w + j)] - 128.f)));
            img[4 * (i * w + j) + 1] = clamp(round(
                y[i * w + j]
                - 0.344136 * (cb[(i * w + j)] - 128.f)
                - 0.714136 * (cr[(i * w + j)] - 128.f)));
            img[4 * (i * w + j) + 0] = clamp(round(
                y[i * w + j]
                + 1.772 * (cb[(i * w + j)] - 128.f)));
        }
    }
}

void quantize(float block[8][8], uint8_t quality[8]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            block[i][j] = round(block[i][j] / (float)quality[tmax(i, j)]);
        }
    }
}

void dequantize(float block[8][8], uint8_t quality[8]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            block[i][j] = round(block[i][j] / (float)quality[tmax(i, j)]);
        }
    }
}

void dct(int *orig, int w, int h, int bx, int by, int *res, uint8_t quality[8]) {
    float block[8][8];
    float block2[8][8];
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            block[i][j] = (float)orig[(i+by) * w + j+bx] - 128.f;
        }
    }
    for (int u = 0; u < 8; u++) {
        for (int v = 0; v < 8; v++) {
            float sum_x = 0;
            float alpha_u = 1.f, alpha_v = 1.f;
            if (u == 0) alpha_u = 0.7071067812; // 1/sqrt(2)
            if (v == 0) alpha_v = 0.7071067812;
            for (int x = 0; x < 8; x++) {
                float sum_y = 0;
                for (int y = 0; y < 8; y++) {
                    sum_y += block[y][x] * cos((2*x+1) * u * PI * 0.0625) * cos((2*y+1) * v * PI * 0.0625);
                }
                sum_x += sum_y;
            }
            block2[v][u] = 0.25 * alpha_u * alpha_v * sum_x;
        }
    }

    quantize(block2, quality);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            res[(i + by) * w + j + bx] = block2[i][j];
        }
    }
}

void idct(int *orig, int w, int h, int bx, int by, int *res, uint8_t quality[8]) {
    float block[8][8];
    float block2[8][8];
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            block2[i][j] = res[(i+by) * w + j+bx];
        }
    }
    dequantize(block2, quality);
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            float sum_u = 0;
            for (int u = 0; u < 8; u++) {
                float sum_v = 0;
                for (int v = 0; v < 8; v++) {
                    float alpha_u = 1.f, alpha_v = 1.f;
                    if (u == 0) alpha_u = 0.7071067812; // 1/sqrt(2)
                    if (v == 0) alpha_v = 0.7071067812;
                    sum_v += alpha_u * alpha_v * block2[v][u] * cos((2*x+1) * u * PI * 0.0625) * cos((2*y+1) * v * PI * 0.0625);
                }
                sum_u += sum_v;
            }
            block[y][x] = 0.25 * sum_u;
        }
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            orig[(i+by) * w + j+bx] = (int)(block[i][j] + 128);
        }
    }
}

void Image::compress(uint8_t quality[8]) {
    int *img_y = (int*)malloc(sizeof(int) * w * h);
    int *img_cr = (int*)malloc(sizeof(int) * w * h);
    int *img_cb = (int*)malloc(sizeof(int) * w * h);
    int *res_y = (int*)malloc(sizeof(int) * w * h);
    int *res_cr = (int*)malloc(sizeof(int) * w * h);
    int *res_cb = (int*)malloc(sizeof(int) * w * h);

    //comprime
    rgb_to_ycc(this->img, img_y, img_cb, img_cr, w, h);

    for (int i = 0; i < (h >> 3); i++) {
        for (int j = 0; j < (w >> 3); j++) {
            dct(img_y, w, h, j << 3, i << 3, res_y, quality);
            dct(img_cr, w, h, j << 3, i << 3, res_cr, quality);
            dct(img_cb, w, h, j << 3, i << 3, res_cb, quality);
        }
    }

    //descomprime
    for (int i = 0; i < (h >> 3); i++) {
        for (int j = 0; j < (w >> 3); j++) {
            idct(img_y, w, h, j << 3, i << 3, res_y, quality);
            idct(img_cr, w, h, j << 3, i << 3, res_cr, quality);
            idct(img_cb, w, h, j << 3, i << 3, res_cb, quality);
        }
    }

    ycc_to_rgb(this->img, img_y, img_cb, img_cr, w, h);
    free(img_y);
    free(img_cb);
    free(img_cr);
    free(res_y);
    free(res_cb);
    free(res_cr);
}
