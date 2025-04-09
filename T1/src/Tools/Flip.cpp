#include "Tool.h"
#include "Flip.h"

#include "../gl_canvas2d.h"
#include "../Layer.h"
#include "../Image.h"
#include "../colors.h"
#include "../Button.h"
#include "../Slider.h"

Flip::Flip(int sw, int sh) {
    for (int i = 0; i < 2; i++)
        bt_orientation[i] = new Button(16 + i * 128, sh - 48, 128, 32);
    bt_orientation[0]->changeText("Horizontal");
    bt_orientation[1]->changeText("Vertical");
}

Flip::~Flip() {
    delete bt_orientation[0];
    delete bt_orientation[1];
}

void Flip::renderOptions(int sw, int sh) {
    CV::color(0.25, 0.25, 0.25);
    CV::rectFill(0, sh - 80, sw, sh);
    for (int i = 0; i < 2; i++)
        bt_orientation[i]->Render();
    CV::text(16, sh - 52, "Orientacao:");
}

bool Flip::checkOptions(int sw, int sh, Mouse mouse) {
    int bt_status = bt_orientation[0]->checkClick(mouse);
    if (bt_status == 1) params[0] = 0;
    bt_status = bt_orientation[1]->checkClick(mouse);
    if (bt_status == 1) params[0] = 1;
    bt_orientation[0]->select(!params[0]);
    bt_orientation[1]->select(params[0]);
    return mouse.y > sh - 80;
}

void Flip::execute(Mouse mouse, Canvas *canvas, Layer *layer, rgb_color *fg, rgb_color *bg) {
    if (mouse.state != 0) return;
    int real_x = canvas->get_x() + layer->get_x();
    int real_y = canvas->get_y() + layer->get_y();
    int w = layer->getImage()->get_w();
    int h = layer->getImage()->get_h();
    if (mouse.x < real_x || mouse.x > real_x + w) return;
    if (mouse.y < real_y || mouse.y > real_y + h) return;
    if (params[0]) layer->getImage()->flip_v();
    else layer->getImage()->flip_h();
    canvas->update();
}

void Flip::changePosition(int sw, int sh) {
    for (int i = 0; i < 2; i++)
        bt_orientation[i]->changePosition(16 + i * 128, sh - 48);
}
