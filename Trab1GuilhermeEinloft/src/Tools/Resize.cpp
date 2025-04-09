#include "Tool.h"
#include "Resize.h"

#include "../gl_canvas2d.h"
#include "../Layer.h"
#include "../Image.h"
#include "../colors.h"
#include "../Button.h"
#include "../Slider.h"

Resize::Resize() {
}

void Resize::renderOptions(int sw, int sh) {
    CV::color(0.25, 0.25, 0.25);
    CV::rectFill(0, sh - 80, sw, sh);
}

bool Resize::checkOptions(int sh, Mouse mouse) {
    return mouse.y > sh - 80;
}

void Resize::execute(Mouse mouse, Canvas *canvas, Layer *layer, rgb_color *fg, rgb_color *bg) {
    int real_x = canvas->get_x() + layer->get_x();
    int real_y = canvas->get_y() + layer->get_y();
    int w = layer->getImage()->get_w();
    int h = layer->getImage()->get_h();
    if (mouse.state == 0) {
        params[0] = RESIZE_NO;
        if (mouse.x > real_x - 8 && mouse.x < real_x + 8)
            params[0] |= RESIZE_MX;
        else if (mouse.x > real_x + w - 8 && mouse.x < real_x + w + 8)
            params[0] |= RESIZE_PX;
        if (mouse.y > real_y - 8 && mouse.y < real_y + 8)
            params[0] |= RESIZE_MY;
        else if (mouse.y > real_y + h - 8 && mouse.y < real_y + h + 8)
            params[0] |= RESIZE_PY;
        if (mouse.x <= real_x - 8 || mouse.x >= real_x + w + 8
            || mouse.y <= real_y - 8 || mouse.y >= real_y + h + 8)
            params[0] = RESIZE_NO;
    }
    if (mouse.l) {
        if ((params[0] & RESIZE_MX) == RESIZE_MX)
            params[1] = w - mouse.x + real_x;
        else if ((params[0] & RESIZE_PX) == RESIZE_PX)
            params[1] = mouse.x - real_x;
        else params[1] = w;
        if ((params[0] & RESIZE_MY) == RESIZE_MY)
            params[2] = h - mouse.y + real_y;
        else if ((params[0] & RESIZE_PY) == RESIZE_PY)
            params[2] = mouse.y - real_y;
        else params[2] = h;
    }
    if (mouse.state == 1) {
        layer->getImage()->resize(params[1], params[2]);
        if ((params[0] & RESIZE_MX) == RESIZE_MX)
            layer->set_x(mouse.x - canvas->get_x());
        if ((params[0] & RESIZE_MY) == RESIZE_MY)
            layer->set_y(mouse.y - canvas->get_y());
        params[0] = RESIZE_NO;
        canvas->update();
    }
    (void)fg;
    (void)bg;
}

void Resize::changePosition(int sh) {
    (void)sh;
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
    if (!params[0]) {
        params[1] = w;
        params[2] = h;
    }
    CV::color(0.0f, 0.0f, 0.0f);
    CV::rect(fake_x, fake_y - 1, fake_x + params[1] + 1, fake_y + params[2]);
    CV::rectFill(fake_x - 5, fake_y - 5, fake_x + 5, fake_y + 5);
    CV::rectFill(fake_x - 5, fake_y + params[2] - 5, fake_x + 5, fake_y + params[2] + 5);
    CV::rectFill(fake_x + params[1] - 5, fake_y - 5, fake_x + params[1] + 5, fake_y + 5);
    CV::rectFill(fake_x + params[1] - 5, fake_y + params[2] - 5, fake_x + params[1] + 5, fake_y + params[2] + 5);
    CV::rectFill(fake_x + params[1]/2 - 5, fake_y - 5, fake_x + params[1]/2 + 5, fake_y + 5);
    CV::rectFill(fake_x + params[1]/2 - 5, fake_y + params[2] - 5, fake_x + params[1]/2 + 5, fake_y + params[2] + 5);
    CV::rectFill(fake_x - 5, fake_y + params[2]/2 - 5, fake_x + 5, fake_y + params[2]/2 + 5);
    CV::rectFill(fake_x + params[1] - 5, fake_y + params[2]/2 - 5, fake_x + params[1] + 5, fake_y + params[2]/2 + 5);
    CV::color(1.0f, 1.0f, 1.0f);
    CV::rect(fake_x - 1, fake_y - 2, fake_x + params[1] + 2, fake_y + params[2] + 1);
    CV::rectFill(fake_x - 4, fake_y - 4, fake_x + 4, fake_y + 4);
    CV::rectFill(fake_x - 4, fake_y + params[2] - 4, fake_x + 4, fake_y + params[2] + 4);
    CV::rectFill(fake_x + params[1] - 4, fake_y - 4, fake_x + params[1] + 4, fake_y + 4);
    CV::rectFill(fake_x + params[1] - 4, fake_y + params[2] - 4, fake_x + params[1] + 4, fake_y + params[2] + 4);
    CV::rectFill(fake_x + params[1]/2 - 4, fake_y - 4, fake_x + params[1]/2 + 4, fake_y + 4);
    CV::rectFill(fake_x + params[1]/2 - 4, fake_y + params[2] - 4, fake_x + params[1]/2 + 4, fake_y + params[2] + 4);
    CV::rectFill(fake_x - 4, fake_y + params[2]/2 - 4, fake_x + 4, fake_y + params[2]/2 + 4);
    CV::rectFill(fake_x + params[1] - 4, fake_y + params[2]/2 - 4, fake_x + params[1] + 4, fake_y + params[2]/2 + 4);
}
