#include <cglm/cam.h>
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "hazlab2.h"


// the reference UP vector
static vec3 up_axis = {(GLfloat)0.0, (GLfloat)1.0, (GLfloat)0.0};


// The user's state
static vec3   user_position = {(GLfloat)0.0, (GLfloat)0.0, (GLfloat)0.0};
static double user_angle    = 0.0;
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


// Maps keys to actions
void process_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		user_angle-= DELTA_ROTATION;
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		user_angle+= DELTA_ROTATION;
	}

	GLfloat delta_x = cos(user_angle) / DELTA_TRANSLATION_DIVISOR;
	GLfloat delta_z = sin(user_angle) / DELTA_TRANSLATION_DIVISOR;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		user_position[0]+= delta_x;
		user_position[2]+= delta_z;
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		user_position[0]-= delta_x;
		user_position[2]-= delta_z;
	}

	glm_look(
		user_position,
		(vec3){delta_x, (GLfloat)0.0, delta_z},
		up_axis,
		current_transform_matrix
	);
}
