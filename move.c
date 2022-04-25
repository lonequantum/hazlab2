#include <cglm/cam.h>
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "hazlab2.h"


// the reference UP vector
static vec3 up_axis = {(GLfloat)0.0, (GLfloat)1.0, (GLfloat)0.0};


// The user's state
static VECTOR3 user_position = {.x = (GLfloat)0.0, .y = (GLfloat)0.0, .z = (GLfloat)0.0};
static float   user_angle    = (GLfloat)0.0;
static mat4 current_transform_matrix = {
	(GLfloat)1.0, (GLfloat)0.0, (GLfloat)0.0, (GLfloat)0.0, // starts with the identity matrix
	(GLfloat)0.0, (GLfloat)1.0, (GLfloat)0.0, (GLfloat)0.0,
	(GLfloat)0.0, (GLfloat)0.0, (GLfloat)1.0, (GLfloat)0.0,
	(GLfloat)0.0, (GLfloat)0.0, (GLfloat)0.0, (GLfloat)1.0
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

	float delta_x = cos(user_angle) / DELTA_TRANSLATION_DIVISOR;
	float delta_z = sin(user_angle) / DELTA_TRANSLATION_DIVISOR;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		user_position.x+= delta_x;
		user_position.z+= delta_z;
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		user_position.x-= delta_x;
		user_position.z-= delta_z;
	}

	glm_look(
		(vec3){user_position.x, (GLfloat)0.0, user_position.z},
		(vec3){delta_x, (GLfloat)0.0, delta_z},
		up_axis,
		current_transform_matrix
	);
}
