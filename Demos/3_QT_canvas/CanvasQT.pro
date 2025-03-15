QT += widgets

SOURCES += main.cpp \
           mainwindow.cpp \
           glCanvas2d.cpp \
		   myCanvas.cpp

HEADERS += \
           mainwindow.h \
		   glCanvas2d.h

target.path = $$[QT_INSTALL_EXAMPLES]/opengl/qopenglwidget

INSTALLS += target

#A seguinte linha deve ser descomentada para uso de funcoes da biblioteca GLU. 
#A pasta lib é a mesma usada nos demais demos OpenGL
LIBS += -L../lib/ -lGLU32
