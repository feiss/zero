#include "../zero.h"


int main (int argc, char **argv){

	GLFWwindow * win = zero_open("Zero linuxgl - rgb example", 256, 256, 4, 1);

  while(!glfwWindowShouldClose(win)){
		glfwSwapBuffers(win);
  }

  return 0;
}