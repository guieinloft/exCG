#ifndef B_SPLINE_H
#define B_SPLINE_H

#include "Vector2.h"

#define B_SEG 128
#define B_MASK B_SEG-1

void b_spline_generate(Vector2 points[], int n_points, Vector2 b_spline[B_SEG]);
void b_spline_draw(Vector2 b_out[B_SEG], Vector2 b_in[B_SEG]);

#endif
