#include <cglm/cam.h>
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "hazlab2.h"


// the reference UP vector
static vec3 up_axis = {0.0f, 1.0f, 0.0f};


// The user's state
static VECTOR3 user_position = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static float   user_angle    = 0.0f;
static mat4 current_transform_matrix = {
	1.0f, 0.0f, 0.0f, 0.0f, // starts with the identity matrix
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
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
		(vec3){user_position.x, 0.0f, user_position.z},
		(vec3){delta_x, 0.0f, delta_z},
		up_axis,
		current_transform_matrix
	);
}
