#ifndef TOOL_TOOL_H
#define TOOL_TOOL_H

#include "../Mouse.h"
#include "../Canvas.h"
#include "../colors.h"

class Tool {
    public:
    Tool();
    virtual ~Tool();
    virtual void renderOptions(int sw, int sh);
    virtual void changePosition(int sw, int sh);
    virtual void checkOptions(int sw, int sh, Mouse mouse);
    virtual void execute(Mouse mouse, Canvas *canvas, Layer *layer, rgb_color *fg, rgb_color *bg);
    virtual void renderBorder(Canvas *canvas, Layer *layer);

    protected:
    int params[4];
};

#endif
