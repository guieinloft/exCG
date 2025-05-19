#include "Textbox.h"
#include "Button.h"
#include "Mouse.h"

#include <string.h>

Textbox::Textbox(int x, int y) {
    this->x = x;
    this->y = y;
    button = new Button(x, y, 256, 32);
    button->changeText((char*)"");
    strcpy(text, (char*)"");
    text_size = 0;
}

void Textbox::checkMouse(Mouse mouse) {
    int bt_status = button->checkClick(mouse);
    if (bt_status == 1) button->select(true);
    else if (bt_status == -1) button->select(false);
}

bool Textbox::checkKeyboard(int key) {
    if (!button->isSelected()) return false;
    if (key == 13) return true;
    else if (key == 8) {
        text[text_size] = '\0';
        if (text_size > 0) text_size--;
        text[text_size] = '\0';
    }
    else if (key >= 32 && key <= 126) {
        text[text_size] = key;
        if (text_size < 255) text_size++;
        text[text_size] = '\0';
    }
    button->changeText(text);
    return false;
}

void Textbox::Render() {
    button->Render();
}

void Textbox::changePosition(int x, int y) {
    this->x = x;
    this->y = y;
    button->changePosition(x, y);
}

void Textbox::reset() {
    button->changeText((char*)"");
    strcpy(text, (char*)"");
    text_size = 0;
    button->select(false);
}

char *Textbox::getText() {
    return text;
}
