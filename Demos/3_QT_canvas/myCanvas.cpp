
// *********************************************************************
// Canvas para desenho - Versao C.
//  Autor: Cesar Tadeu Pozzer
//         07/2016
//
//  Pode ser utilizada para fazer desenhos ou animacoes, como jogos simples.
//  Tem tratamento de mosue
// *
// *   pozzer@inf.ufsm.br
// *   pozzer3@gmail.com
// *
//

#include "glCanvas2d.h"
#include "mainwindow.h"

#include <math.h>
#include <QMessageBox>
#include <QWheelEvent>


#include <GL/glu.h>

float global = 0;


// *******************************************************************************
//Coloque seu codigo aqui, usando as funcoes da Canvas2D defindas na classe Canvas2D (arquivo glCanvas2d.h).
// *******************************************************************************
void Canvas2D::paintGL() //callback de desenho na canvas. Chamado pelo Timer definido em mainWindow.cpp
{
    clear(0.8, 0.8, 0.8);

    //*******************************************************************************
    //para uso das funcoes da glut, deve-se adicionar a lib no arquivo .pro.
    //*******************************************************************************
    //glMatrixMode(GL_MODELVIEW);
    //gluLookAt(1,1,1, 3,3,3, 4,4,4);
    //glMatrixMode(GL_PROJECTION);
    //gluOrtho2D(1,1,2,3);
    //gluPerspective(50, 1, 1, 100);


    color(1,0,0);
    line(10,10,100,100);

    float x=0, y;
    color(0, 1, 0);
    for(float i=0; i < 68; i+=0.001)
    {
       y = sin(i)*50;
       point((int)x, (int)y+100);
       x+=0.01;
    }

    text(30,30, "Isso eh um teste de escrita");

    circleFill(200,200, 30, 18);

    //desenha um relogio
    color(0,0,0);
    float raio = 200;
    x = (int)(cos(global) * raio);
    y = (int)(sin(global) * raio);
    line(200,200, x+200, y+200);
    global-= 0.01;
}


void Canvas2D::wheelEvent(QWheelEvent *event) //callback de mouse
{
    //printf("\nMouse Wheel event: %d", event->delta() );
    qDebug("Mouse Wheel event %d", event->delta() );
    mw->list->addItem("Mouse Wheel event");
}

void Canvas2D::mousePressEvent(QMouseEvent *event) //callback de mouse
{
    //seta o foco para a canvas2D, desse modo pode-se pegar eventos de teclado dentro da canvas.
    setFocus();

    qDebug("\nMouse Press: %d %d", event->x(), event->y() );
    if(event->buttons() == Qt::RightButton)
        qDebug( "Only right button" );
    mw->list->addItem("Mouse Click event");
}

void Canvas2D::keyReleaseEvent(QKeyEvent* event)
{
    qDebug("\nTecla solta  %d", event->key() );
}

void Canvas2D::mouseReleaseEvent(QMouseEvent *event) //callback de mouse
{
    qDebug("\nMouse Release: %d %d", event->x(), event->y());
}

void Canvas2D::mouseMoveEvent(QMouseEvent * event) //callback de mouse
{
    qDebug("\nMouse Move: %d %d", event->x(), event->y());
    mw->list->addItem("Mouse Move event");
}

//callback para botao definido na mainWindow.
void Canvas2D::showMsg()
{
    QMessageBox* msg = new QMessageBox(this);
    msg->setText("Msg GlWidget");
    msg->show();
}

void Canvas2D::keyPressEvent(QKeyEvent* event)
{
    qDebug("\nTecla pessionada  %d", event->key() );
}
