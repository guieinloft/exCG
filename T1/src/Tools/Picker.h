#ifndef TOOL_PICKER_H
#define TOOL_PICKER_H

#include "Tool.h"
#include "../colors.h"
#include "../Slider.h"
#include "../Button.h"

class Picker : public Tool {
    public:
    Picker();
    void renderOptions(int sw, int sh);
    bool checkOptions(int sh, Mouse mouse);
    void execute(Mouse mouse, Canvas *canvas, Layer *layer, rgb_color *fg, rgb_color *bg);
    void changePosition(int sh);
};

#endif
