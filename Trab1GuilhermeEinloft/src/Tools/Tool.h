#ifndef TOOL_TOOL_H
#define TOOL_TOOL_H

#include "../Mouse.h"
#include "../Canvas.h"
#include "../colors.h"

class Tool {
    public:
    //construtor (toda ferramenta executa)
    Tool();
    //destrutor
    virtual ~Tool();
    //mostra opções da ferramenta na tela
    virtual void renderOptions(int sw, int sh);
    //muda posição das opções da ferramenta
    virtual void changePosition(int sh);
    //verifica se o mouse clicou em alguma opção
    virtual bool checkOptions(int sh, Mouse mouse);
    //executa a ferramenta
    virtual void execute(Mouse mouse, Canvas *canvas, Layer *layer, rgb_color *fg, rgb_color *bg);
    //renderiza borda da camada
    virtual void renderBorder(Canvas *canvas, Layer *layer);

    protected:
    int params[4];
};

#endif
