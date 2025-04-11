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
    //construtor
    ToolList();
    //destrutor
    ~ToolList();
    //mostra a lista de ferramentas na tela
    void Render();
    //verifica para cada botão se foi pressionado
    //seleciona o botão pressionado (caso algum tenha sido)
    bool checkMouse(Mouse mouse);
    //retorna o botão selecionado
    int getSelectedTool();

    private:
    //lista de botões (um para cada ferramenta)
    Button *bt_list[TOOL_NUM];
    //botão selecionado
    int bt_selected;
};

#endif
