#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

GLFWwindow *zero_open(char *title, int w, int h, int d, int s);
int zero_update(void *fb, void *pal);
void zero_close() {};

typedef void(*Zero_Handler_Func)(int type, int param1, int param2);
void zero_events(Zero_Handler_Func handler);

#define MAKE_RGB(r,g,b) (((r)<<16) | ((g)<<8) | (b))

enum {
	RESIZE = 1,
	KEY_DOWN,
	KEY_UP,
	MOUSE_DOWN,
	MOUSE_UP,
	MOUSE_MOVE,
	MOUSE_ENTER,
	MOUSE_LEAVE
} Zero_Event_Types;

enum {
	SHIFT = 1,
	CONTROL = 2,
	ALT = 4,
	SUPER = 8
} Zero_Key_Modifier;

enum {
	MOUSE_LEFT = 0,
	MOUSE_RIGHT,
	MOUSE_MIDDLE
} Zero_Mouse_Button;


// implementation


unsigned int fb_texture;
static GLFWwindow *window;
static int width, height, depth, scale;
static GLuint vertexbuffer;
static GLuint shaderProgram;
static Zero_Handler_Func __event_handler;


//static void *active_fb = 0;
//static void *active_pal = 0;

void initShaders() {
	GLuint vertex_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_id = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vertex_shader =
		"#version 400\n"
		"layout(location = 0) in vec2 inPos;"
		"void main() {"
		"  gl_Position = vec4(inPos, 0.0, 1.0);"
		"}";

	const char* fragment_shader =
		"#version 400\n"
		"uniform sampler2D texture;"
		"uniform vec2 size;"
		"out vec4 outColor;"
		"void main() {"
		"  vec2 uv = gl_FragCoord.xy / size;"
		"  uv.y = 1.0 - uv.y;"
		"  outColor = texture2D(texture, uv);"
		"}";


	glShaderSource(vertex_id, 1, &vertex_shader, NULL);
	glCompileShader(vertex_id);

	// Check Vertex Shader
	GLint result = GL_FALSE;
	int infoLength;
	glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertex_id, GL_INFO_LOG_LENGTH, &infoLength);
	if ( infoLength > 0 ){
		printf("Vertex shader error\n");
	}

	glShaderSource(fragment_id, 1, &fragment_shader , NULL);
	glCompileShader(fragment_id);

	glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragment_id, GL_INFO_LOG_LENGTH, &infoLength);
	if ( infoLength > 0 ){
		printf("Fragment shader error\n");
	}

	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vertex_id);
	glAttachShader(ProgramID, fragment_id);
	glLinkProgram(ProgramID);
	glUseProgram(ProgramID);

	GLint loc = glGetUniformLocation(ProgramID, "size");
	glUniform2f(loc, width * scale, height * scale);

/*	
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &infoLength);
	if ( infoLength > 0 ){
		printf("Program shader error\n");
	}

	glDetachShader(ProgramID, vertex_id);
	glDetachShader(ProgramID, fragment_id);
	
	glDeleteShader(vertex_id);
	glDeleteShader(fragment_id);
	return ProgramID;
*/
}
//

GLFWwindow *zero_open(char *title, int w, int h, int d, int s){

  width = w;
  height = h;
  depth = d == 1 ? 1 : 4;
  scale = s > 1 ? s : 1;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	window = glfwCreateWindow(width * scale, height * scale, title, NULL, NULL);

  const GLFWvidmode* video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  glfwSetWindowPos(window, 
  	video_mode->width / 2 - (w * s) / 2, 
  	video_mode->height / 2 - (h * s) / 2);
  glfwSetCursorPos(window, w * s / 2, h * s / 2);

  glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE; 
  if (glewInit() != GLEW_OK) { return 0; }

  initShaders();

  glClearColor(1.0, 0.0, 0.0, 1.0);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	static const GLfloat g_vertex_buffer_data[] = {
	   -4.0f, -4.0f, 0.0f,
	   4.0f, -4.0f, 0.0f,
	   0.0f,  4.0f, 0.0f,
	};
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glDisable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &fb_texture);
  glBindTexture(GL_TEXTURE_2D, fb_texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight,
  //              0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);  

  return window;
}

int zero_update(void *fb, void *pal){

	glClear(GL_COLOR_BUFFER_BIT);

	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, fb);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, fb);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

static void keyboard_handler(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (!__event_handler) return;
	__event_handler(action == GLFW_RELEASE ? KEY_UP : KEY_DOWN, scancode, mods);
}
static void mouse_position_handler(GLFWwindow* window, double x, double y){
	if (!__event_handler) return;
	__event_handler(MOUSE_MOVE, (int)x, (int)y);
}
static void mouse_enter_handler(GLFWwindow* window, int entered) {
	if (!__event_handler) return;
	__event_handler(entered ? MOUSE_ENTER : MOUSE_LEAVE, 0, 0);
}
static void mouse_button_handler(GLFWwindow* window, int button, int action, int mods) {
	if (!__event_handler) return;
	__event_handler(action == GLFW_PRESS ? MOUSE_DOWN : MOUSE_UP, button, mods);
}

void zero_events(Zero_Handler_Func handler) {
	__event_handler = handler;
	if (handler) {
		glfwSetKeyCallback(window, keyboard_handler);
		glfwSetCursorPosCallback(window, mouse_position_handler);
		glfwSetCursorEnterCallback(window, mouse_enter_handler);
		glfwSetMouseButtonCallback(window, mouse_button_handler);
	}
}

