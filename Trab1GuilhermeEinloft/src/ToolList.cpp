#include "ToolList.h"
#include "Button.h"
#include "Mouse.h"
#include "colors.h"
#include "gl_canvas2d.h"

#include "Tools/Tool.h"
#include "Tools/Pencil.h"

#include <stdio.h>

#define TL_W 1

ToolList::ToolList() {
    for (int i = 0; i < TOOL_NUM; i++) {
        bt_list[i] = new Button(16 + (i % TL_W) * 32, 16 + (i / TL_W) * 48, 32, 32);
    }
    //load icons
    bt_list[TOOL_PENCIL]->loadIcons("./Trab1GuilhermeEinloft/images/icons/pencil.bmp");
    bt_list[TOOL_ERASER]->loadIcons("./Trab1GuilhermeEinloft/images/icons/eraser.bmp");
    bt_list[TOOL_MOVE]->loadIcons("./Trab1GuilhermeEinloft/images/icons/move.bmp");
    bt_list[TOOL_RESIZE]->loadIcons("./Trab1GuilhermeEinloft/images/icons/resize.bmp");
    bt_list[TOOL_ROTATE]->loadIcons("./Trab1GuilhermeEinloft/images/icons/rotate.bmp");
    bt_list[TOOL_FLIP]->loadIcons("./Trab1GuilhermeEinloft/images/icons/flip.bmp");
    bt_list[TOOL_PICKER]->loadIcons("./Trab1GuilhermeEinloft/images/icons/picker.bmp");
    bt_selected = 0;
    bt_list[0]->select(true);
}

ToolList::~ToolList() {
    for (int i = 0; i < TOOL_NUM; i++) {
        delete bt_list[i];
    }
}

void ToolList::Render() {
    extern int screenHeight;
    CV::color(0.25, 0.25, 0.25);
    //CV::rectFill(0, 0, 32 + 32 * TL_W, 128 + TOOL_NUM / TL_W);
    CV::rectFill(0, 0, 32 + 32 * TL_W, screenHeight);
    for (int i = 0; i < TOOL_NUM; i++) {
        bt_list[i]->Render();
    }
}

bool ToolList::checkMouse(Mouse mouse) {
    for (int i = 0; i < TOOL_NUM; i++) {
        int bt_status = bt_list[i]->checkClick(mouse);
        if (bt_status == 1) bt_selected = i;
    }
    for (int i = 0; i < TOOL_NUM; i++) {
        bt_list[i]->select(i == bt_selected);
    }
    return mouse.x < 64;
}

int ToolList::getSelectedTool() {
    return this->bt_selected;
}
