#include <cglm/cam.h>
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "hazlab2.h"


// the reference UP vector
static vec3 up_axis = {(GLfloat)0.0, (GLfloat)1.0, (GLfloat)0.0};


// The camera/user's state
static vec3   user_position = {(GLfloat)0.0, USER_Y, (GLfloat)0.0};
static double user_angle    = DELTA_ROTATION * 4;
static mat4   current_transform_matrix = {
	{(GLfloat)1.0, (GLfloat)0.0, (GLfloat)0.0, (GLfloat)0.0}, // starts with the identity matrix
	{(GLfloat)0.0, (GLfloat)1.0, (GLfloat)0.0, (GLfloat)0.0},
	{(GLfloat)0.0, (GLfloat)0.0, (GLfloat)1.0, (GLfloat)0.0},
	{(GLfloat)0.0, (GLfloat)0.0, (GLfloat)0.0, (GLfloat)1.0}
};
mat4 *get_current_transform_matrix(void)
{
	return &current_transform_matrix;
}
vec3 *get_user_position(void)
{
	return &user_position;
}


#define user_x user_position[0]
#define user_z user_position[2]
#define K(x) (int)((x) + size / (GLfloat)2.0)


// Maps keys to actions
void process_input(GLFWwindow *const window, const int size, const int *const maze_lines[])
{
	int i = K(user_z);
	int j = K(user_x);
	if (i == 0 || j == 0 || i == size - 1 || j == size - 1)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		user_angle-= DELTA_ROTATION;
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		user_angle+= DELTA_ROTATION;

	GLfloat delta_x = cos(user_angle) / DELTA_TRANSLATION_DIVISOR;
	GLfloat delta_z = sin(user_angle) / DELTA_TRANSLATION_DIVISOR;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		GLfloat new_x = user_x + delta_x;
		GLfloat new_z = user_z + delta_z;

		if (maze_lines[K(new_z)][j])
			user_z = new_z;

		if (maze_lines[i][K(new_x)])
			user_x = new_x;
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		GLfloat new_x = user_x - delta_x;
		GLfloat new_z = user_z - delta_z;

		if (maze_lines[K(new_z)][j])
			user_z = new_z;

		if (maze_lines[i][K(new_x)])
			user_x = new_x;
	}

	glm_look(
		user_position,
		(vec3){delta_x, LOOK_AT_DELTA_Y, delta_z},
		up_axis,
		current_transform_matrix
	);
}
