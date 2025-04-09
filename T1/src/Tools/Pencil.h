#ifndef TOOL_PENCIL_H
#define TOOL_PENCIL_H

#include "Tool.h"
#include "../colors.h"
#include "../Slider.h"
#include "../Button.h"

#define abslt(x) (x * (x >= 0) - x * (x < 0))

enum PencilFormats {
    FORMAT_CIRCLE,
    FORMAT_SQUARE,
    FORMAT_DIAMOND,
    FORMAT_NUM
};

class Pencil : public Tool {
    public:
    Pencil(int sh);
    ~Pencil();
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
