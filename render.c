#include <cglm/cglm.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "hazlab2.h"
#define ERROR_MSG_MAX_LEN 512


// Main GL data
static GLuint  vbo, vao, ebo;
static GLuint  shader_program;


static GLsizei n_elements;
static mat4    projection_matrix;


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
	out vec3 position_untransformed;\
	uniform mat4 transform;\
	uniform mat4 projection;\
	void main()\
	{\
		gl_Position = projection * transform * vec4(position, 1.0);\
		position_untransformed = position;\
	}";


// Source of the geometry shader
static const GLchar *geometry_shader_src = "#version 330 core\n\
	layout (triangles) in;\
	layout (triangle_strip, max_vertices = 3) out;\
	in vec3 position_untransformed[];\
	out vec3 normal;\
	out vec3 fragment_position;\
	void main()\
	{\
		vec3 a = position_untransformed[1] - position_untransformed[0];\
		vec3 b = position_untransformed[2] - position_untransformed[0];\
		normal = normalize(cross(a, b));\
		gl_Position = gl_in[0].gl_Position;\
		fragment_position = position_untransformed[0];\
		EmitVertex();\
		gl_Position = gl_in[1].gl_Position;\
		fragment_position = position_untransformed[1];\
		EmitVertex();\
		gl_Position = gl_in[2].gl_Position;\
		fragment_position = position_untransformed[2];\
		EmitVertex();\
		EndPrimitive();\
	}";


// Source of the fragment shader
static const GLchar *fragment_shader_src = "#version 330 core\n\
	in vec3 normal;\
	in vec3 fragment_position;\
	out vec4 color;\
	uniform vec3 light_position;\
	void main()\
	{\
		vec3 light_color = vec3(1.0, 1.0, 1.0);\
		vec3 walls_color = vec3(0.2, 0.4, 0.8);\
		float ambient_coeff = 0.1;\
		\
		vec3 light = normalize(fragment_position - light_position);\
		vec3 diffuse = max(dot(normal, light), 0.0) * light_color;\
		color = vec4((ambient_coeff + diffuse) * walls_color, 1.0f);\
	}";


// Compiles the given shader and checks the compilation status
static bool compile_shader(const GLuint shader_handle)
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
bool prepare_scene(const VERTICES_SET data)
{
	n_elements = data.i_length;

	glClearColor(BACKGROUND_COLOR);

	// Creates the vertex shader
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
	if (!compile_shader(vertex_shader))
		return false;

	// Creates the geometry shader
	GLuint geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry_shader, 1, &geometry_shader_src, NULL);
	if (!compile_shader(geometry_shader))
		return false;

	// Creates the fragment shader
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
	if (!compile_shader(fragment_shader))
		return false;

	// Links shaders
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, geometry_shader);
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
	glEnableVertexAttribArray(0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	// Closes what is no longer needed
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glDeleteShader(geometry_shader);

	// defines the fixed projection matrix
	glm_perspective(FOV_ANGLE_RAD, (float)WINDOW_WIDTH / WINDOW_HEIGHT, CLIP_MIN, CLIP_MAX, projection_matrix);

#ifndef NDEBUG
	prepare_scene_debug_adds();
#endif
	return true;
}


// Draws the whole scene
void draw_scene(void)
{
	static mat4 *transform_matrix;
	transform_matrix = get_current_transform_matrix();

	static vec3 *light_position;
	light_position = get_user_position();

	glUseProgram(shader_program);

	GLint transform_location = glGetUniformLocation(shader_program, "transform");
	glUniformMatrix4fv(transform_location, 1, GL_FALSE, (GLfloat *)transform_matrix);

	GLint projection_location = glGetUniformLocation(shader_program, "projection");
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, (GLfloat *)projection_matrix);

	GLint light_position_location = glGetUniformLocation(shader_program, "light_position");
	glUniform3fv(light_position_location, 1, (GLfloat *)light_position);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, n_elements, GL_UNSIGNED_INT, 0);
}
