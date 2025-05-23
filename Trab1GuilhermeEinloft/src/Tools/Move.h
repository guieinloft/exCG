#ifndef TOOL_MOVE_H
#define TOOL_MOVE_H

#include "Tool.h"
#include "../colors.h"
#include "../Slider.h"
#include "../Button.h"

class Move : public Tool {
    public:
    Move();
    void renderOptions(int sw, int sh);
    bool checkOptions(int sh, Mouse mouse);
    void execute(Mouse mouse, Canvas *canvas, Layer *layer, rgb_color *fg, rgb_color *bg);
    void changePosition(int sh);
};

#endif
