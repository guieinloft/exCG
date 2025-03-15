/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QListWidget>
#include <QSpinBox>

#include "glCanvas2d.h"


QT_FORWARD_DECLARE_CLASS(QOpenGLWidget)

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    bool timerEnabled() const { return m_timer->isActive(); }
    void resizeEvent(QResizeEvent *);

    QTimer   *m_timer;
    QSpinBox *refreshRate;
    QListWidget *list;


//Callbacks de Widgets
private slots:
    void updateIntervalChanged(int value);
    void showMsg();
    void sliderChanged(int i);
    void checkBoxChanged(bool b);

};

#endif
