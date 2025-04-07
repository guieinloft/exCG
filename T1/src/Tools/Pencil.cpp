#include "Tool.h"
#include "Pencil.h"

#include "../gl_canvas2d.h"
#include "../Layer.h"
#include "../Image.h"
#include "../colors.h"
#include "../Button.h"
#include "../Slider.h"

Pencil::Pencil(int sw, int sh) {
    sl_size = new Slider(16, sh - 32);
    sl_quality = new Slider(288, sh - 32);
    for (int i = 0; i < FORMAT_NUM; i++)
        bt_formats[i] = new Button(560 + i * 32, sh - 48, 32, 32);
    bt_formats[FORMAT_CIRCLE]->loadIcons("images/icons/pencil_circle.bmp");
    bt_formats[FORMAT_SQUARE]->loadIcons("images/icons/pencil_square.bmp");
    bt_formats[FORMAT_DIAMOND]->loadIcons("images/icons/pencil_diamond.bmp");
    bt_formats[0]->select(true);
}

Pencil::~Pencil() {
    delete sl_size;
    delete sl_quality;
    for (int i = 0; i < FORMAT_NUM; i++)
        delete bt_formats[i];
}

void Pencil::renderOptions(int sw, int sh) {
    CV::color(0.25, 0.25, 0.25);
    CV::rectFill(0, sh - 80, sw, sh);
    sl_size->Render();
    sl_quality->Render();
    for (int i = 0; i < FORMAT_NUM; i++)
        bt_formats[i]->Render();
    CV::text(16, sh - 52, "Tamanho:");
    CV::text(288, sh - 52, "Qualidade:");
    CV::text(560, sh - 52, "Formato:");
}

void Pencil::checkOptions(int sw, int sh, Mouse mouse) {
    sl_size->checkMouse(mouse);
    params[1] = sl_size->getParam();
    sl_quality->checkMouse(mouse);
    params[2] = sl_quality->getParam();
    for (int i = 0; i < FORMAT_NUM; i++) {
        int bt_status = bt_formats[i]->checkClick(mouse);
        if (bt_status == 1) params[0] = i;
    }
    for (int i = 0; i < FORMAT_NUM; i++) {
        bt_formats[i]->select(i == params[0]);
    }
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

void paintSquare(int x, int y, int d, Image *image, rgb_color c) {
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            image->put_pixel(x - j + d/2, y - i + d/2, c.r, c.g, c.b, 255, false);
        }
    }
}

void paintDiamond(int x, int y, int rad, Image *image, rgb_color c) {
    for (int i = -rad; i <= rad; i++) {
        for (int j = -rad; j <= rad; j++) {
            if (abslt(i) + abslt(j) <= rad) {
                image->put_pixel(x + j, y + i, c.r, c.g, c.b, 255, false);
            }
        }
    }
}

void Pencil::execute(Mouse mouse, Canvas *canvas, Layer *layer, rgb_color *fg, rgb_color *bg) {
    Image *image = layer->getImage();
    int real_x = mouse.x - canvas->get_x() - layer->get_x();
    int real_y = mouse.y - canvas->get_y() - layer->get_y();
    int real_xp = mouse.xp - canvas->get_x() - layer->get_x();
    int real_yp = mouse.yp - canvas->get_y() - layer->get_y();
    int diameter = params[1] + 1;
    int rad = (diameter + 1)/2;
    int quality = params[2] + 1;
    //int rad = 3;
    if (mouse.l) {
        for (int i = 0; i < quality; i++) {
            int cx = (real_xp * i + real_x * (quality - i))/quality;
            int cy = (real_yp * i + real_y * (quality - i))/quality;
            if (params[0] == FORMAT_SQUARE)
                paintSquare(cx, cy, diameter, image, *fg);
            else if (params[0] == FORMAT_DIAMOND)
                paintDiamond(cx, cy, rad, image, *fg);
            else
                paintCircle(cx, cy, rad, image, *fg);
        }
        canvas->update();
    }
    else if (mouse.r) {
        for (int i = 0; i < quality; i++) {
            int cx = (real_xp * i + real_x * (quality - i))/quality;
            int cy = (real_yp * i + real_y * (quality - i))/quality;
            if (params[0] == FORMAT_SQUARE)
                paintSquare(cx, cy, params[1] + 1, image, *bg);
            else
                paintCircle(cx, cy, rad, image, *bg);
        }
        canvas->update();
    }
}

void Pencil::changePosition(int sw, int sh) {
    sl_size->changePosition(16, sh - 32);
    sl_quality->changePosition(288, sh - 32);
    for (int i = 0; i < FORMAT_NUM; i++)
        bt_formats[i]->changePosition(560 + i * 32, sh - 48);
}
