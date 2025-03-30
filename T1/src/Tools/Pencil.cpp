#include "Tool.h"
#include "Pencil.h"

#include "../gl_canvas2d.h"
#include "../Layer.h"
#include "../Image.h"
#include "../colors.h"

void Pencil::renderOptions(int sw, int sh) {
    CV::color(0.25, 0.25, 0.25);
    CV::rectFill(0, sh - 64, sw, sh);
}

void Pencil::checkOptions(int sw, int sh, Mouse mouse) {
}

void paintCircle(int x, int y, int rad, Image *image, rgb_color c) {
    for (int i = -rad; i <= rad; i++) {
        for (int j = -rad; j <= rad; j++) {
            if (i * i + j * j <= rad * rad + rad) {
                image->put_pixel(x + j, y + i, c.r, c.g, c.b, 255, false);
            }
        }
    }
}

void Pencil::execute(Mouse mouse, Canvas *canvas, Layer *layer, rgb_color fg, rgb_color bg) {
    Image *image = layer->getImage();
    int real_x = mouse.x - canvas->get_x() - layer->get_x();
    int real_y = mouse.y - canvas->get_y() - layer->get_y();
    //int rad = ((int)params[1] + 1)/2;
    int rad = 3;
    printf("\nIMAGE: %p", image);
    switch (this->params[0]) {
        default:
        if (mouse.l)
            paintCircle(real_x, real_y, rad, image, fg);
        else if (mouse.r)
            paintCircle(real_x, real_y, rad, image, bg);
        break;
    }
}

