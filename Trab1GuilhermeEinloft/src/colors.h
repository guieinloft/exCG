#ifndef COLORS_H
#define COLORS_H

#include <stdint.h>

//cor rgba
struct rgb_color_t {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

//cor hsla
struct hsl_color_t {
    float h;
    float s;
    float l;
    float a;
};

typedef struct rgb_color_t rgb_color;
typedef struct hsl_color_t hsl_color;

//transforma de hsl pra rgb
rgb_color hsl_to_rgb(hsl_color hsl);
//transforma de rgb pra hsl
hsl_color rgb_to_hsl(rgb_color rgb);

#endif
