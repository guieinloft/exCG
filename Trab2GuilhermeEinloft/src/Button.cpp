#include "Button.h"
#include "gl_canvas2d.h"
#include "Image.h"
#include "Mouse.h"

#include <string.h>

Button::Button(int x, int y, int w, int h) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    strcpy(this->text, "");
    this->pressed = false;
    this->selected = false;
    this->hovered = false;
    this->iconCur = false;
}

int Button::checkClick(Mouse mouse) {
    this->hovered = true;
    if (mouse.x < this->x) this->hovered = false;
    if (mouse.x > this->x + this->w) this->hovered = false;
    if (mouse.y < this->y) this->hovered = false;
    if (mouse.y > this->y + this->h) this->hovered = false;

    if (mouse.l && mouse.state == 0) {
        if (this->hovered) {
            this->pressed = true;
            return 1; //click in button
        }
        else return -1; //click not in button
    }
    else if (mouse.state == 1) {
        return 0; //click released
    }
    else return -2; //not clicked
}

void Button::Render() {
    if (this->selected) CV::color(0.15, 0.15, 0.15);
    else if (this->hovered) CV::color(0.35, 0.35, 0.35);
    else CV::color(0.25, 0.25, 0.25);
    CV::rectFill(this->x, this->y, this->x + this->w, this->y + this->h);
    CV::color(0.5, 0.5, 0.5);
    CV::rect(this->x, this->y, this->x + this->w, this->y + this->h);
    CV::rect(this->x + 1, this->y + 1, this->x + this->w - 1, this->y + this->h - 1);
    if (iconCur)
        this->icon2.render(this->x, this->y, 255);
    else
        this->icon1.render(this->x, this->y, 255);
    CV::text(this->x + 4, this->y + this->h - 4, this->text);
}

bool Button::isPressed() {
    bool p = this->pressed;
    this->pressed = false;
    return p;
}

bool Button::isSelected() {
    return this->selected;
}

void Button::select(bool s) {
    this->selected = s;
}

void Button::loadIcons(const char *path1, const char *path2) {
    icon1.bmp_load(path1);
    if (path2 != NULL)
        icon2.bmp_load(path2);
}

void Button::changeIcon() {
    iconCur = !iconCur;
}

void Button::changeText(const char *text) {
    strcpy(this->text, text);
}

void Button::changePosition(int x, int y) {
    this->x = x;
    this->y = y;
}
