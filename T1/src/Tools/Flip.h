#ifndef TOOL_FLIP_H
#define TOOL_FLIP_H

#include "Tool.h"
#include "../colors.h"
#include "../Slider.h"
#include "../Button.h"

class Flip : public Tool {
    public:
    Flip(int sw, int sh);
    void renderOptions(int sw, int sh);
    void checkOptions(int sw, int sh, Mouse mouse);
    void execute(Mouse mouse, Canvas *canvas, Layer *layer, rgb_color *fg, rgb_color *bg);
    void changePosition(int sw, int sh);
    
    private:
    Button *bt_orientation[2];
};

#endif
