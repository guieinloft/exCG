#ifndef TOOL_ERASER_H
#define TOOL_ERASER_H

#include "Tool.h"
#include "Pencil.h"
#include "../colors.h"
#include "../Slider.h"
#include "../Button.h"

class Eraser : public Tool {
    public:
    Eraser(int sh);
    ~Eraser();
    void renderOptions(int sw, int sh);
    bool checkOptions(int sh, Mouse mouse);
    void execute(Mouse mouse, Canvas *canvas, Layer *layer, rgb_color *fg, rgb_color *bg);
    void changePosition(int sh);
    
    private:
    Slider *sl_size;
    Slider *sl_quality;
    Button *bt_formats[FORMAT_NUM];
};

#endif
