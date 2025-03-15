/*******************************************************************
*
*	Descrição: Exemplo de utilização da biblioteca glfw.
*
*	Autor: Alex Thomas Almeida Frasson
*          Cesar Pozzer
*
*	Universidade Federal de Santa Maria - 2012/1
*
*   -------------------------------------------
*
*   OBSERVACAO IMPORTANTE:
*   As libs devem estar na seguinte ordem (LINKER SETTINS)
*   1) glfw3dll.a
*   2) libglfw3.a
*   3) libopengl32.a

*******************************************************************/

#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#define WIDTH 500
#define HEIGHT 500

GLFWwindow* window;

void render(void);
void cursorPosCallback(GLFWwindow *w, double x, double y);
void mouseButtonCallback(GLFWwindow *w, int button, int action, int mods);
void scrollCallBack(GLFWwindow *w, double xoffset, double yoffset);
void keyCallBack(GLFWwindow *w, int,int,int,int);

void init(void);


void render(void)
{
    //float ratio;
    int width, height;

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);


    glBegin(GL_TRIANGLES);
      glColor3f(1.f, 0.f, 0.f);
      glVertex3f(-0.6f, -0.4f, 0.f);
      glColor3f(0.f, 1.f, 0.f);
      glVertex3f(0.6f, -0.4f, 0.f);
      glColor3f(0.f, 0.f, 1.f);
      glVertex3f(0.f, 0.6f, 0.f);
    glEnd();

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void initGLFW(void)
{
   int glfwVersion[3];
   glfwGetVersion(&glfwVersion[0], &glfwVersion[1], &glfwVersion[2]);
   fprintf(stdout, "GLFW Version %d.%d rev.%d\n", glfwVersion[0], glfwVersion[1], glfwVersion[2]);

	//GLFW
	if (!glfwInit())
		exit(1);
	window = glfwCreateWindow(WIDTH, HEIGHT, "Demo glfw Basic", NULL, NULL);
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

void cursorPosCallback(GLFWwindow *w, double x, double y)
{
    printf("\nmouse moveu");
}

void mouseButtonCallback(GLFWwindow *w, int button, int action, int mods)
{
    printf("\nbotao do mouse");
}

void scrollCallBack(GLFWwindow *w, double xoffset, double yoffset)
{
    printf("\nscrool do mouse");
}

void keyCallBack(GLFWwindow *w, int a,int b,int c,int d)
{
    printf("\nkeyboard %d %d %d %d", a, b, c,d);
}


int main()
{
	initGLFW();

	while(glfwWindowShouldClose(window) == 0 && glfwGetKey(window, GLFW_KEY_ESCAPE) == 0)
	{
		render();
	}

    glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

