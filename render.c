#include <cglm/cglm.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>
#include "hazlab2.h"
#define ERROR_MSG_MAX_LEN 512


// Main GL data
static GLuint  vbo, vao, ebo;
static GLuint  shader_program;
static GLsizei n_elements;


// Resets main GL data
void delete_GL_data(void)
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteProgram(shader_program);
}


// Source of the vertex shader
static const GLchar *vertex_shader_src = "#version 330 core\n\
	layout (location = 0) in vec3 position;\
	uniform mat4 transform;\
	void main()\
	{\
		gl_Position = transform * vec4(position.x, position.y, position.z, 1.0);\
	}";


// Source of the fragment shader
static const GLchar *fragment_shader_src = "#version 330 core\n\
	out vec4 color;\
	void main()\
	{\
		color = vec4(0.5f, 0.5f, 0.5f, 1.0f);\
	}";


// Compiles the given shader and checks the compilation status
static bool compile_shader(GLuint shader_handle)
{
	glCompileShader(shader_handle);

	GLint success;
	GLchar info[ERROR_MSG_MAX_LEN];
	glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader_handle, ERROR_MSG_MAX_LEN, NULL, info);
		eprintf("could not compile shader: %s\n", info);
		return false;
	}

	return true;
}


// Creates the data to be sent to the graphic card
bool prepare_scene(VERTICES_SET data)
{
	n_elements = data.i_length;

	// Creates the vertex shader
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
	if (!compile_shader(vertex_shader))
		return false;

	// Creates the fragment shader
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
	if (!compile_shader(fragment_shader))
		return false;

	// Links shaders
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	GLint success;
	GLchar info[ERROR_MSG_MAX_LEN];
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader_program, ERROR_MSG_MAX_LEN, NULL, info);
		eprintf("could not link shader program: %s\n", info);
		return false;
	}

	// Creates buffers
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	// Sends data
	glBufferData(GL_ARRAY_BUFFER, data.v_length * sizeof(VERTEX), data.vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.i_length * sizeof(GLuint), data.indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glPolygonMode(GL_BACK, GL_LINE);
	glPolygonMode(GL_FRONT, GL_FILL);

	// Closes what is no longer needed
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return true;
}


// Draws the whole scene
void draw_scene(void)
{
	glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	static mat4 transform_matrix = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	glUseProgram(shader_program);
	GLint transform_location = glGetUniformLocation(shader_program, "transform");
	glUniformMatrix4fv(transform_location, 1, GL_FALSE, (GLfloat*)transform_matrix);

	glDrawElements(GL_TRIANGLES, n_elements, GL_UNSIGNED_INT, 0);
}
