#ifndef TOOL_H
#define TOOL_H

#include "../Mouse.h"
#include "../Canvas.h"

class Tool {
    public:
    Tool();
    virtual void renderOptions(int sw, int sh);
    virtual void checkOptions(int sw, int sh, Mouse mouse);
    virtual void execute(Mouse mouse, Canvas *canvas);

    protected:
    uint8_t params[3];
};

#endif
