#include "Tool.h"
#include "Resize.h"

#include "../gl_canvas2d.h"
#include "../Layer.h"
#include "../Image.h"
#include "../colors.h"
#include "../Button.h"
#include "../Slider.h"

Resize::Resize(int sw, int sh) {
}

void Resize::renderOptions(int sw, int sh) {
    CV::color(0.25, 0.25, 0.25);
    CV::rectFill(0, sh - 80, sw, sh);
}

void Resize::checkOptions(int sw, int sh, Mouse mouse) {
}

void Resize::execute(Mouse mouse, Canvas *canvas, Layer *layer, rgb_color *fg, rgb_color *bg) {
    int real_x = canvas->get_x() + layer->get_x();
    int real_y = canvas->get_y() + layer->get_y();
    int w = layer->getImage()->get_w();
    int h = layer->getImage()->get_h();
    if (mouse.state == 0) {
        if (mouse.x > real_x - 8 && mouse.x < real_x + 8
            && mouse.y > real_y - 8 && mouse.y < real_y + 8) {
            params[0] = RESIZE_MX | RESIZE_MY;
            printf("\nMX MY");
        }
        else if (mouse.x > real_x + w - 8 && mouse.x < real_x + w + 8
            && mouse.y > real_y - 8 && mouse.y < real_y + 8) {
            params[0] = RESIZE_PX | RESIZE_MY;
            printf("\nPX MY");
        }
        else if (mouse.x > real_x - 8 && mouse.x < real_x + 8
            && mouse.y > real_y + h - 8 && mouse.y < real_y + h + 8) {
            params[0] = RESIZE_MX | RESIZE_PY;
            printf("\nMX PY");
        }
        else if (mouse.x > real_x + w - 8 && mouse.x < real_x + w + 8
            && mouse.y > real_y + h - 8 && mouse.y < real_y + h + 8) {
            params[0] = RESIZE_PX | RESIZE_PY;
            printf("\nPX PY");
        }
        else params[0] = RESIZE_NO;
    }
    if (mouse.l) {
        switch (params[0]) {
            case (RESIZE_PX | RESIZE_PY):
            params[1] = mouse.x - real_x;
            params[2] = mouse.y - real_y;
            break;
            case (RESIZE_PX | RESIZE_MY):
            params[1] = mouse.x - real_x;
            params[2] = h - mouse.y + real_y;
            break;
            case (RESIZE_MX | RESIZE_PY):
            params[1] = w - mouse.x + real_x;
            params[2] = mouse.y - real_y;
            break;
            case (RESIZE_MX | RESIZE_MY):
            params[1] = w - mouse.x + real_x;
            params[2] = h - mouse.y + real_y;
            break;
            default:
            params[1] = w;
            params[2] = h;
            break;
        }
    }
    if (mouse.state == 1) {
        layer->getImage()->resize(params[1], params[2]);
        if ((params[0] & RESIZE_MX) == RESIZE_MX)
            layer->set_x(mouse.x - canvas->get_x());
        if ((params[0] & RESIZE_MY) == RESIZE_MY)
            layer->set_y(mouse.y - canvas->get_y());
        params[0] = RESIZE_NO;
    }
}

void Resize::changePosition(int sw, int sh) {
}

void Resize::renderBorder(Canvas *canvas, Layer *layer) {
    int real_x = canvas->get_x() + layer->get_x();
    int real_y = canvas->get_y() + layer->get_y();
    int w = layer->getImage()->get_w();
    int h = layer->getImage()->get_h();
    int fake_x = real_x, fake_y = real_y;
    if ((params[0] & RESIZE_MX) == RESIZE_MX)
        fake_x = real_x - params[1] + w;
    if ((params[0] & RESIZE_MY) == RESIZE_MY)
        fake_y = real_y - params[2] + h;
    CV::color(0.0f, 0.0f, 0.0f);
    CV::rect(fake_x - 1, fake_y - 1, fake_x + params[1], fake_y + params[2]);
    CV::rectFill(fake_x - 5, fake_y - 5, fake_x + 5, fake_y + 5);
    CV::rectFill(fake_x - 5, fake_y + params[2] - 5, fake_x + 5, fake_y + params[2] + 5);
    CV::rectFill(fake_x + params[1] - 5, fake_y - 5, fake_x + params[1] + 5, fake_y + 5);
    CV::rectFill(fake_x + params[1] - 5, fake_y + params[2] - 5, fake_x + params[1] + 5, fake_y + params[2] + 5);
    CV::color(1.0f, 1.0f, 1.0f);
    CV::rect(fake_x - 2, fake_y - 2, fake_x + params[1] + 1, fake_y + params[2] + 1);
    CV::rectFill(fake_x - 4, fake_y - 4, fake_x + 4, fake_y + 4);
    CV::rectFill(fake_x - 4, fake_y + params[2] - 4, fake_x + 4, fake_y + params[2] + 4);
    CV::rectFill(fake_x + params[1] - 4, fake_y - 4, fake_x + params[1] + 4, fake_y + 4);
    CV::rectFill(fake_x + params[1] - 4, fake_y + params[2] - 4, fake_x + params[1] + 4, fake_y + params[2] + 4);
}
