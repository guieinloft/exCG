#ifndef COLORS_H
#define COLORS_H

#include <stdint.h>

struct rgb_color_t {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

struct hsl_color_t {
    float h;
    float s;
    float l;
    float a;
};

struct hsv_color_t {
    float h;
    float s;
    float v;
    float a;
};

typedef struct rgb_color_t rgb_color;
typedef struct hsl_color_t hsl_color;
typedef struct hsv_color_t hsv_color;

rgb_color hsl_to_rgb(hsl_color hsl);
hsl_color rgb_to_hsl(rgb_color rgb);

rgb_color hsv_to_rgb(hsv_color hsv);
hsv_color rgb_to_hsv(rgb_color rgb);

#endif
