#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow *zero_open(char *title, int w, int h, int d, int s);
int zero_update(void *fb, void *pal);
void zero_close() {};

// implementation


static int width, height, depth, scale;
//static void *active_fb = 0;
//static void *active_pal = 0;

GLFWwindow *zero_open(char *title, int w, int h, int d, int s){

  width = w;
  height = h;
  depth = d == 1 ? 1 : 4;

  glfwInit();
	GLFWwindow *window = glfwCreateWindow(w * s, h * s, title, NULL, NULL);

  const GLFWvidmode* video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  glfwSetWindowPos(window, 
  	video_mode->width / 2 - (w * s) / 2, 
  	video_mode->height / 2 - (h * s) / 2);
  glfwSetCursorPos(window, w * s / 2, h * s / 2);

  glfwMakeContextCurrent(window);
  return window;
}

int zero_update(void *fb, void *pal){

}