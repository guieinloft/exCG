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
    ~ToolList();
    void Render();
    bool checkMouse(Mouse mouse);
    int getSelectedTool();

    private:
    Button *bt_list[TOOL_NUM];
    int bt_selected;
};

#endif
