#ifndef LEFTBAR_H
#define LEFTBAR_H

#include "Button.h"
#include "Mouse.h"
#include "Tools/Tool.h"

enum Tools {
    TOOL_PENCIL,
    TOOL_ERASER,
    TOOL_MOVE,
    TOOL_RESIZE,
    TOOL_ROTATE,
    TOOL_FLIP,
    TOOL_PICKER,
    TOOL_NUM
};

class ToolList {
    public:
    ToolList();
    void Render();
    void checkMouse(Mouse mouse, Canvas *canvas, Layer *layer, rgb_color fg, rgb_color bg);
    int getSelectedTool();

    private:
    Button *bt_list[TOOL_NUM];
    int bt_selected;

    Tool *tools[TOOL_NUM];
};

#endif
