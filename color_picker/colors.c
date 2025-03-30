#include "colors.h"
#include <math.h>
#include <stdio.h>

void hue_to_rgb(float *r, float *g, float *b, float h, float c) {
    int hh = h/60;
    float x = c * (1 - round(fmod(hh, 2) - 1));
    switch (hh) {
        case 0:
        *r = c;
        *g = x;
        *b = 0;
        break;

        case 1:
        *r = x;
        *g = c;
        *b = 0;
        break;

        case 2:
        *r = 0;
        *g = c;
        *b = x;
        break;

        case 3:
        *r = 0;
        *g = x;
        *b = c;
        break;

        case 4:
        *r = x;
        *g = 0;
        *b = c;
        break;

        case 5:
        *r = c;
        *g = 0;
        *b = x;
        break;

        default:
        break;
    }
}

rgb_color hsl_to_rgb(hsl_color hsl) {
    float c = (1 - round(2*hsl.l - 1)) * hsl.s;

    float r, g, b;
    hue_to_rgb(&r, &g, &b, hsl.h, c);

    float m = hsl.l - c/2;
    rgb_color rgb = {(r + m) * 255, (g + m) * 255, (b + m) * 255};
    //printf("\n%d %d %d", rgb.r, rgb.g, rgb.b);
    return rgb;
}

hsl_color rgb_to_hsl(rgb_color rgb) {
    hsl_color hsl;
    float r = rgb.r/255.0, g = rgb.g/255.0, b = rgb.b/255.0;
    
    float x_max = fmax(r, fmax(g, b));
    float x_min = fmin(r, fmin(g, b));
    float c = x_max - x_min;

    if (x_max == x_min) hsl.h = 0;
    else if (x_max == r) hsl.h = 60 * (fmod((g - b)/c, 6));
    else if (x_max == g) hsl.h = 60 * ((b - r)/c + 2);
    else if (x_max == b) hsl.h = 60 * ((r - g)/c + 4);

    hsl.l = (x_max + x_min)/2;

    if (hsl.l == 0 || hsl.l == 1) hsl.s = 0;
    else hsl.s = c / (1 - round(2*hsl.l - 1));
    //printf("\n%f %f %f", hsl.h, hsl.l, hsl.s);
}

rgb_color hsv_to_rgb(hsv_color hsv) {
    float c = hsv.v * hsv.s;

    float r, g, b;
    hue_to_rgb(&r, &g, &b, hsv.h, c);

    float m = hsv.v - c;
    rgb_color rgb = {(r + m) * 255, (g + m) * 255, (b + m) * 255};
    //printf("\n%d %d %d", rgb.r, rgb.g, rgb.b);
    return rgb;
}

hsv_color rgb_to_hsv(rgb_color rgb) {
    hsv_color hsv;
    float r = rgb.r/255.0, g = rgb.g/255.0, b = rgb.b/255.0;
    
    hsv.v = fmax(r, fmax(g, b));
    float x_min = fmin(r, fmin(g, b));
    float c = hsv.v - x_min;

    if (hsv.v == x_min) hsv.h = 0;
    else if (hsv.v == r) hsv.h = 60 * (fmod((g - b)/c, 6));
    else if (hsv.v == g) hsv.h = 60 * ((b - r)/c + 2);
    else if (hsv.v == b) hsv.h = 60 * ((r - g)/c + 4);

    if (hsv.v == 0) hsv.s = 0;
    else hsv.s = c/hsv.v;
    //printf("\n%f %f %f", hsv.h, hsv.v, hsv.s);
}

