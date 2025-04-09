#include "colors.h"
#include <math.h>
#include <stdio.h>

float hue_to_rgb(float t1, float t2, float h) {
    h = fmod(h/360 + 1, 1);
    if (6 * h < 1) return t2 + (t1 - t2) * 6 * h;
    if (2 * h < 1) return t1;
    if (3 * h < 2) return t2 + (t1 - t2) * (0.666 - h) * 6;
    return t2;
}

rgb_color hsl_to_rgb(hsl_color hsl) {
    if (hsl.s == 0) {
        rgb_color rgb = {(uint8_t)(hsl.l * 255), (uint8_t)(hsl.l * 255), (uint8_t)(hsl.l * 255), (uint8_t)(hsl.a * 255)};
        return rgb;
    }

    float t1 = ((hsl.l * (1 + hsl.s)) * (hsl.l < 0.5)) + ((hsl.l + hsl.s - hsl.l * hsl.s) * (hsl.l >= 0.5));
    float t2 = 2 * hsl.l - t1;
    rgb_color rgb = {(uint8_t)(hue_to_rgb(t1, t2, hsl.h + 120) * 255),
        (uint8_t)(hue_to_rgb(t1, t2, hsl.h) * 255), (uint8_t)(hue_to_rgb(t1, t2, hsl.h - 120) * 255), (uint8_t)(hsl.a * 255)};
    //printf("\n%d %d %d", rgb.r, rgb.g, rgb.b);
    return rgb;
}

hsl_color rgb_to_hsl(rgb_color rgb) {
    hsl_color hsl;
    float r = rgb.r/255.0, g = rgb.g/255.0, b = rgb.b/255.0;
    
    float x_max = fmax(r, fmax(g, b));
    float x_min = fmin(r, fmin(g, b));
    float c = x_max - x_min;
    hsl.h = hsl.s = hsl.l = (x_max + x_min)/2;
    hsl.h *= 360;

    if (x_max == x_min) hsl.h = hsl.s = 0;
    else if (x_max == r) hsl.h = 60 * (fmod((g - b)/c, 6));
    else if (x_max == g) hsl.h = 60 * ((b - r)/c + 2);
    else if (x_max == b) hsl.h = 60 * ((r - g)/c + 4);

    hsl.l = (x_max + x_min)/2;

    if (hsl.l == 0 || hsl.l == 1) hsl.s = 0;
    else hsl.s = (x_max - hsl.l)/fmin(hsl.l, 1-hsl.l);
    //printf("\n%f %f %f", hsl.h, hsl.l, hsl.s);
    hsl.a = rgb.a / 255.0;
    hsl.h = fmod(hsl.h + 360, 360);
    return hsl;
}

