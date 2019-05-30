#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

GLFWwindow *zero_open(char *title, int w, int h, int d, int s);
int zero_update(void *fb, void *pal);
void zero_close() {};

// implementation

unsigned int fb_texture;

static int width, height, depth, scale;
static GLuint vertexbuffer;
//static void *active_fb = 0;
//static void *active_pal = 0;

GLFWwindow *zero_open(char *title, int w, int h, int d, int s){

  width = w;
  height = h;
  depth = d == 1 ? 1 : 4;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(w * s, h * s, title, NULL, NULL);

  const GLFWvidmode* video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  glfwSetWindowPos(window, 
  	video_mode->width / 2 - (w * s) / 2, 
  	video_mode->height / 2 - (h * s) / 2);
  glfwSetCursorPos(window, w * s / 2, h * s / 2);

  glfwMakeContextCurrent(window);
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	static const GLfloat g_vertex_buffer_data[] = {
	   -1.0f, -1.0f, 0.0f,
	   1.0f, -1.0f, 0.0f,
	   0.0f,  1.0f, 0.0f,
	};
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	/*
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  //glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &fb_texture);
  glBindTexture(GL_TEXTURE_2D, fb_texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight,
//                0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);  
  */
  return window;
}

int zero_update(void *fb, void *pal){

	//glTexSubImage2D(TEXTURE_2D, 0, 0, 0, width, height, LUMINANCE, UNSIGNED_BYTE, this.fb.data);
/*
	glTexSubImage2D(TEXTURE_2D, 0, 0, 0, width, height, depth == 3 ? GL_RGB : GL_RGBA, UNSIGNED_BYTE, fb);

	useProgram(this.programInfo.program);
	twgl.setBuffersAndAttributes(this.gl, this.programInfo, this.bufferInfo);
	twgl.setUniforms(this.programInfo, this.uniforms);
*/

/*	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
	*/
}


static GLuint loadShaders() {
	GLuint vertex_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_id = glCreateShader(GL_FRAGMENT_SHADER);

	static const char *vertex_shader = "";
	static const char *fragment_shader = "";


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
}
