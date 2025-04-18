#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "gl_canvas2d.h"

#include "Image.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif
#ifndef M_E
#define M_E 2.7182818284590452354f
#endif

//funções auxiliares
inline int sign(float a) {
    return (a > 0) - (a < 0);
}
inline float min(float a, float b) {
    return (a > b) * b + (a <= b) * a;
}
inline float max(float a, float b) {
    return (a < b) * b + (a >= b) * a;
}

Image::Image() {
    w = 0;
    h = 0;
    img = NULL;
}

Image::~Image() {
    close_image();
}

void Image::close_image() {
    if (this->img == NULL) return;
    free(this->img);
}

bool Image::bmp_load(const char *path) {
    //open file
    FILE *file = fopen(path, "rb");
    if (file == NULL) return false;

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
    if (this->img == NULL) return false;
    fseek(file, offset, SEEK_SET);
    int padding = (4 - this->w * bpp) % 4;
    for (int i = this->h - 1; i > -1; i--) {
        for (uint32_t j = 0; j < this->w; j++) {
            fread(&this->img[(i * this->w + j) * 4], sizeof(uint8_t), bpp, file);
            for (int k = bpp; k < 4; k++)
                this->img[(i * this->w + j) * 4 + k] = 255;
        }
        fseek(file, padding, SEEK_CUR);
    }

    //close file
    fclose(file);
    return true;
}

void Image::bmp_save(const char *path) {
    //open file
    FILE *file = fopen(path, "wb");
    if (file == NULL) return;

    //write bmp header
    fwrite("BM", sizeof(char), 2, file);
    uint32_t fsize = 54 + w * h * 4;
    fwrite(&fsize, sizeof(uint32_t), 1, file);
    uint32_t temp_zero = 0;
    uint16_t temp_one = 1;
    uint16_t temp_32 = 32;
    fwrite(&temp_zero, sizeof(uint32_t), 1, file);
    uint32_t offset = 54;
    fwrite(&offset, sizeof(uint32_t), 1, file);

    //write dib header
    uint32_t hsize = 40;
    fwrite(&hsize, sizeof(uint32_t), 1, file);
    fwrite(&w, sizeof(uint32_t), 1, file);
    fwrite(&h, sizeof(uint32_t), 1, file);
    fwrite(&temp_one, sizeof(uint16_t), 1, file);
    fwrite(&temp_32, sizeof(uint16_t), 1, file);
    fwrite(&temp_zero, sizeof(uint32_t), 1, file);
    fsize = w * h * 4;
    fwrite(&fsize, sizeof(uint32_t), 1, file);
    fwrite(&w, sizeof(uint32_t), 1, file);
    fwrite(&h, sizeof(uint32_t), 1, file);
    fwrite(&temp_zero, sizeof(uint32_t), 1, file);
    fwrite(&temp_zero, sizeof(uint32_t), 1, file);

    //write image
    for (int i = h - 1; i >= 0; i--) {
        fwrite(&(img[i * w * 4]), sizeof(uint8_t), 4 * w, file);
    }

    //close file
    fclose(file);
}

void Image::file_save(FILE *file) {
    fwrite(&w, sizeof(uint32_t), 1, file);
    fwrite(&h, sizeof(uint32_t), 1, file);
    fwrite(img, sizeof(uint8_t), 4 * w * h, file);
}

void Image::file_load(FILE *file) {
    close_image();
    fread(&w, sizeof(uint32_t), 1, file);
    fread(&h, sizeof(uint32_t), 1, file);
    img = (uint8_t*)malloc(sizeof(uint8_t) * 4 * w * h);
    fread(img, sizeof(uint8_t), 4 * w * h, file);
}

void Image::render(float x, float y, int o) {
    for (uint32_t i = 0; i < this->w * this->h; i++) {
        int base = 4 * i;
        if (this->img[base+3]) {
            CV::color(this->img[base+2]/255.0, this->img[base+1]/255.0 ,this->img[base]/255.0, this->img[base+3] * o /255.0/255.0);
            CV::point(x + i % this->w, y + i / this->w);
        }
    }
}

void Image::flip_h() {
    uint8_t *temp = (uint8_t*)malloc(sizeof(uint8_t) * 4);
    for (uint32_t i = 0; i < this->h; i++) {
        for (uint32_t j = 0; j < this->w / 2; j++) {
            memcpy(temp, &this->img[(i * this->w + j) * 4], 4);
            memcpy(&this->img[(i * this->w + j) * 4], &this->img[(i * this->w + this->w - 1 - j) * 4], 4);
            memcpy(&this->img[(i * this->w + this->w - 1 - j) * 4], temp, 4);
        }
    }
    free(temp);
}

void Image::flip_v() {
    uint8_t *temp = (uint8_t*)malloc(sizeof(uint8_t) * this->w * 4);
    for (uint32_t i = 0; i < this->h / 2; i++) {
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
    if (x >= 0 && x < (int)this->w && y >= 0 && y < (int)this->h) {
        int base = (y * this->w + x) * 4;
        if (b >= 0) this->img[base] = b * (a * blend + 255 * !blend)/255.0 + this->img[base] * (255 - a)/255.0 * blend;
        if (g >= 0) this->img[base+1] = g * (a * blend + 255 * !blend)/255.0 + this->img[base+1] * (255 - a)/255.0 * blend;
        if (r >= 0) this->img[base+2] = r * (a * blend + 255 * !blend)/255.0 + this->img[base+2] * (255 - a)/255.0 * blend;
        if (a >= 0) this->img[base+3] = a * !blend + 255 * (blend && (this->img[base+3] >= 255 - a)) + (this->img[base+3] + a) * (blend && (this->img[base+3] < 255 - a));
    }
}

void Image::get_pixel(int x, int y, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a) {
    if (x >= 0 && x < (int)this->w && y >= 0 && y < (int)this->h) {
        int base = (y * this->w + x) * 4;
        *r = this->img[base+2];
        *g = this->img[base+1];
        *b = this->img[base+0];
        *a = this->img[base+3];
    }
    else {
        *r = 0;
        *g = 0;
        *b = 0;
        *a = 0;
    }
}

void Image::blend(Image *src, int x, int y, int sx, int sy, int o) {
    for (int i = max(0, (y - sy)); i < min(this->h, (src->h - sy + y)); i++) {
        for (int j = max(0, (x - sx)); j < min(this->w, (src->w - sx + x)); j++) {
            int b1 = (i * this->w + j) * 4;
            int b2 = ((i - y + sy) * src->w + (j - x + sx)) * 4;
            float aa = src->img[b2+3] * o/255.0;
            float ab = this->img[b1+3] * (1 - aa/255.0);
            this->img[b1] = (src->img[b2] * aa + this->img[b1] * ab) / (aa + ab);
            this->img[b1+1] = (src->img[b2+1] * aa + this->img[b1+1] * ab) / (aa + ab);
            this->img[b1+2] = (src->img[b2+2] * aa + this->img[b1+2] * ab) / (aa + ab);
            this->img[b1+3] = aa + ab;
        }
    }
}

void Image::copy(Image *src) {
    this->close_image();
    this->w = src->w;
    this->h = src->h;
    this->img = (uint8_t*)malloc(sizeof(uint8_t) * src->w * src->h * 4);
    memcpy(this->img, src->img, sizeof(uint8_t) * src->w * src->h * 4);
}

void Image::clear_image(int new_w, int new_h) {
    this->close_image();

    this->w = new_w;
    this->h = new_h;
    this->img = (uint8_t*)malloc(sizeof(uint8_t) * this->w * this->h * 4);

    for (uint32_t i = 0; i < this->h; i++) {
        for (uint32_t j = 0; j < this->w; j++) {
            for (int k = 0; k < 4; k++) {
                this->img[(i * this->w + j) * 4 + k] = 0;
            }
        }
    }
}

void Image::rotate(float rad, int *offx, int *offy) {
    float cosrad = cos(rad);
    float sinrad = sin(rad);
    int x1 = 0, y1 = 0;
    int x2 = w * cosrad, y2 = w * sin(rad);
    int x3 = -(h * sin(rad)), y3 = h * cosrad;
    int x4 = w * cosrad - h * sinrad, y4 = w * sinrad + h * cosrad;
    int x_min = min(x1, min(x2, min(x3, x4)));
    int y_min = min(y1, min(y2, min(y3, y4)));
    int x_max = max(x1, max(x2, max(x3, x4)));
    int y_max = max(y1, max(y2, max(y3, y4)));
    int nw = x_max - x_min, nh = y_max - y_min;
    int old_cx = w / 2, old_cy = h / 2;
    int new_cx = nw / 2, new_cy = nh / 2;
    uint8_t *new_img = (uint8_t*)malloc(sizeof(uint8_t) * nw * nh * 4);
    for (int i = y_min; i < y_max; i++) {
        for (int j = x_min; j < x_max; j++) {
            int base_x = (int)(j * cosrad - i * -sinrad);
            int base_y = (int)(j * -sinrad + i * cosrad);
            int b1 = ((i - y_min) * nw + (j - x_min)) * 4;
            //printf("%d\n", b1);
            if (base_x >= 0 && base_x < (int)w && base_y >= 0 && base_y < (int)h) {
                int b2 = (base_y * w + base_x) * 4;
                new_img[b1+0] = img[b2+0];
                new_img[b1+1] = img[b2+1];
                new_img[b1+2] = img[b2+2];
                new_img[b1+3] = img[b2+3];
            }
            else {
                new_img[b1+0] = 0;
                new_img[b1+1] = 0;
                new_img[b1+2] = 0;
                new_img[b1+3] = 0;
            }
        }
    }
    close_image();
    this->img = new_img;
    this->w = nw;
    this->h = nh;
    *offx = old_cx - new_cx;
    *offy = old_cy - new_cy;
}

void Image::blur(int radius, bool horizontal, bool vertical) {
    float sigma = fmax(radius/2.0, 1.0);
    float expden = 2 * sigma * sigma;
    int ksize = radius * 2 + 1;
    float sum = 0.0;
    float *img_kernel = (float*)malloc(sizeof(float) * ksize);
    if (img_kernel == NULL) return;
    uint8_t *new_img = (uint8_t*)malloc(sizeof(uint8_t) * this->w * this->h * 4);
    if (new_img == NULL) return;
    uint8_t *new_img2 = (uint8_t*)malloc(sizeof(uint8_t) * this->w * this->h * 4);
    if (new_img2 == NULL) return;
    //define img_kernel values
    for (int i = -radius; i <= radius; i++) {
        float expnum = -(i * i);
        float eexpr = pow(M_E, expnum/expden);
        img_kernel[(i + radius)] = eexpr / (expden * M_PI);
        sum += img_kernel[(i + radius)];
    }
    //normalize img_kernel values
    for (int i = 0; i < ksize; i++) {
        img_kernel[i] /= sum;
    }
    //first pass: horizontal
    if (horizontal) {
        for (int i = 0; i < (int)h; i++) {
            for (int j = 0; j < (int)w; j++) {
                float r = 0.f, g = 0.f, b = 0.f, a = 0.f;
                for (int kj = -radius; kj <= radius; kj++) {
                    float kvalue = img_kernel[(kj + radius)];
                    if (j + kj >= 0 && j + kj < (int)w) {
                        int base = (i * w + j + kj) * 4;
                        r += img[base + 2] * kvalue;
                        g += img[base + 1] * kvalue;
                        b += img[base + 0] * kvalue;
                        a += img[base + 3] * kvalue;
                    }
                }
                int base_n = (i * w + j) * 4;
                new_img[base_n + 2] = (uint8_t)r;
                new_img[base_n + 1] = (uint8_t)g;
                new_img[base_n + 0] = (uint8_t)b;
                new_img[base_n + 3] = (uint8_t)a;
            }
        }
    }
    else {
        memcpy(new_img, this->img, sizeof(uint8_t) * w * h * 4);
    }
    free(this->img);
    //second pass: vertical
    if (vertical) {
        for (int i = 0; i < (int)h; i++) {
            for (int j = 0; j < (int)w; j++) {
                float r = 0.f, g = 0.f, b = 0.f, a = 0.f;

                for (int ki = -radius; ki <= radius; ki++) {
                    float kvalue = img_kernel[(ki + radius)];
                    if (i + ki >= 0 && i + ki < (int)h) {
                        int base = ((i + ki) * w + j) * 4;
                        r += new_img[base + 2] * kvalue;
                        g += new_img[base + 1] * kvalue;
                        b += new_img[base + 0] * kvalue;
                        a += new_img[base + 3] * kvalue;
                    }
                }
                int base_n = (i * w + j) * 4;
                new_img2[base_n + 2] = (uint8_t)r;
                new_img2[base_n + 1] = (uint8_t)g;
                new_img2[base_n + 0] = (uint8_t)b;
                new_img2[base_n + 3] = (uint8_t)a;
            }
        }
    }
    else {
        memcpy(new_img2, new_img, sizeof(uint8_t) * w * h * 4);
    }
    free(img_kernel);
    free(new_img);
    this->img = new_img2;
}
