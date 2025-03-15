/**
*   Autor: Cesar Tadeu Pozzer
*   UFSM - 15/05/2020
*
*   pozzer@inf.ufsm.br
*   pozzer3@gmail.com
*
**/


#include "superficie.h"
#include "gl_canvas2d.h"


Superficie *s;

////////////////////////////////////////////////////////////////////////////////////////
void render(void)
{
    CV::color(1,0,0);

    s->render();

    Sleep(10);  //reduz a velocidade de renderizacao. Nao eh a forma correta de se fazer. Deve-se utilizar funcoes de tempo.
}

int main(void)
{
    int w = 600, h = 600;

    s = new Superficie();

    CV::init(&w, &h, "Projecao em Perspectiva");
    CV::run();
}

