/**
*   Programa para ilustrar os elementos mais basicos do OpenGL e GLFW.
*
*   Autor: Cesar Tadeu Pozzer
*   UFSM - 2024
*
*   pozzer@inf.ufsm.br
*   pozzer3@gmail.com
*
**/


#define GLFW_INCLUDE_GLU

#include "gl_canvas2d.h"


GLFWwindow* window;


//conjunto de cores predefinidas. Pode-se adicionar mais cores.
float Colors[14][3]=
{
    {0, 0, 0}, //Black
    {0.5, 0.5, 0.5}, //Gray
    {1, 0, 0}, //Red
    {0, 1, 0}, //Green
    {0, 0, 1}, //Blue
    {0, 1, 1}, //Cyan
    {1, 0, 1}, //Magenta
    {1, 1, 0}, //Yellow
    {1, 0.5, 0}, //Orange
    {0.5, 0, 0}, //Brown
    {0.5, 0.5, 0}, //Olive
    {0, 0.5, 0.5}, //
    {0.5, 0, 0.5}, //
    {1, 1, 1}, //white
};

void ConvertMouseCoord(int button, int state, int wheel, int direction, int x, int y);

//funcoes de CALLBACK da biblioteca GLFW
void cursorPosCallback(GLFWwindow *w, double x, double y);
void mouseButtonCallback(GLFWwindow *w, int button, int action, int mods);
void scrollCallBack(GLFWwindow *w, double xoffset, double yoffset);
void keyCallBack(GLFWwindow *w, int a,int b,int c,int d);

void render();


void CV::point(float x, float y)
{
   glBegin(GL_POINTS);
      glVertex2d(x, y);
   glEnd();
}

void CV::point(Vector2 p)
{
   glBegin(GL_POINTS);
      glVertex2d(p.x, p.y);
   glEnd();
}

void CV::line( float x1, float y1, float x2, float y2 )
{
   glBegin(GL_LINES);
      glVertex2d(x1, y1);
      glVertex2d(x2, y2);
   glEnd();
}

void CV::rect( float x1, float y1, float x2, float y2 )
{
   glBegin(GL_LINE_LOOP);
      glVertex2d(x1, y1);
      glVertex2d(x1, y2);
      glVertex2d(x2, y2);
      glVertex2d(x2, y1);
   glEnd();
}

void CV::rectFill( float x1, float y1, float x2, float y2 )
{
   glBegin(GL_QUADS);
      glVertex2d(x1, y1);
      glVertex2d(x1, y2);
      glVertex2d(x2, y2);
      glVertex2d(x2, y1);
   glEnd();
}
void CV::rectFill( Vector2 p1, Vector2 p2 )
{
   glBegin(GL_QUADS);
      glVertex2d(p1.x, p1.y);
      glVertex2d(p1.x, p2.y);
      glVertex2d(p2.x, p2.y);
      glVertex2d(p2.x, p1.y);
   glEnd();
}

void CV::polygon(float vx[], float vy[], int elems)
{
   int cont;
   glBegin(GL_LINE_LOOP);
      for(cont=0; cont<elems; cont++)
      {
         glVertex2d(vx[cont], vy[cont]);
      }
   glEnd();
}

void CV::polygonFill(float vx[], float vy[], int elems)
{
   int cont;
   glBegin(GL_POLYGON);
      for(cont=0; cont<elems; cont++)
      {
         glVertex2d(vx[cont], vy[cont]);
      }
   glEnd();

}


void CV::clear(float r, float g, float b)
{
   glClearColor( r, g, b, 1 );
}

void CV::circle( float x, float y, float radius, int div )
{
   float ang = 0, x1, y1;
   float inc = PI_2/div;
   glBegin(GL_LINE_LOOP);
      for(int lado = 1; lado <= div; lado++) //GL_LINE_LOOP desenha um poligono fechado. Liga automaticamente o primeiro e ultimio vertices.
      {
         x1 = (cos(ang)*radius);
         y1 = (sin(ang)*radius);
         glVertex2d(x1+x, y1+y);
         ang+=inc;
      }
   glEnd();
}

void CV::circleFill( float x, float y, float radius, int div )
{
   float ang = 0, x1, y1;
   float inc = PI_2/div;
   glBegin(GL_POLYGON);
      for(int lado = 1; lado <= div; lado++) //GL_POLYGON desenha um poligono CONVEXO preenchido.
      {
         x1 = (cos(ang)*radius);
         y1 = (sin(ang)*radius);
         glVertex2d(x1+x, y1+y);
         ang+=inc;
      }
   glEnd();
}

//coordenada de offset para desenho de objetos.
//nao armazena translacoes cumulativas.
void CV::translate(float offsetX, float offsetY)
{
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslated(offsetX, offsetY, 0);
}

void CV::translate(Vector2 offset)
{
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslated(offset.x, offset.y, 0);
}

void CV::color(float r, float g, float b)
{
   glColor3d(r, g, b);
}

void CV::color(int idx)
{
    glColor3fv(Colors[idx]);
}

void CV::color(float r, float g, float b, float alpha)
{
   glColor4d(r, g, b, alpha);
}

int CV::ConvertMouseCoord( int y )
{
#if Y_CANVAS_CRESCE_PARA_CIMA == 1
   return screenHeight - y; //deve-se inverter a coordenada y do mouse se o y da canvas crescer para cima. O y do mouse sempre cresce para baixo.
#else
   return y;
#endif
}


//funcao chamada sempre que a tela for redimensionada.
void reshape (int w, int h)
{

   screenHeight = h; //atualiza as variaveis da main() com a nova dimensao da tela.
   screenWidth = w;

   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();

   //cria uma projecao ortografica com z entre (-1, 1).
#if Y_CANVAS_CRESCE_PARA_CIMA == 1
   //parametros: left, right, bottom, top
   gluOrtho2D (0.0, w, 0.0, h); //o eixo y cresce para cima.
#else
   //parametros: left, right, bottom, top
   gluOrtho2D (0.0, w, h, 0.0); //o eixo y cresce para baixo
#endif

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity ();
}


////////////////////////////////////////////////////////////////////////////////////////
//  inicializa o OpenGL
////////////////////////////////////////////////////////////////////////////////////////
void CV::init(int w, int h, const char *title)
{
   screenHeight = h; //atualiza as variaveis da main() com a nova dimensao da tela.
   screenWidth = w;

   glClearColor(1,1,1,1);
   glPolygonMode(GL_FRONT, GL_FILL);

   printf("\nGL Version: %s", glGetString(GL_VERSION));

   int glfwVersion[3];
   glfwGetVersion(&glfwVersion[0], &glfwVersion[1], &glfwVersion[2]);
   fprintf(stdout, "\nGLFW Version %d.%d rev.%d\n", glfwVersion[0], glfwVersion[1], glfwVersion[2]);

	//GLFW
	if (!glfwInit())
		exit(1);
	window = glfwCreateWindow(screenWidth, screenHeight, title, NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(1);
	}
	glfwMakeContextCurrent(window);

	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallBack);
	glfwSetKeyCallback(window, keyCallBack);
}


void CV::run()
{
	while(glfwWindowShouldClose(window) == 0 && glfwGetKey(window, GLFW_KEY_ESCAPE) == 0)
	{
        glClear(GL_COLOR_BUFFER_BIT );

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        reshape (screenWidth, screenHeight);

        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
	}

    glfwDestroyWindow(window);
	glfwTerminate();
}

