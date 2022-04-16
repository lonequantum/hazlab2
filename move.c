#include <cglm/cglm.h>
#include "hazlab2.h"


static mat4 current_transform_matrix = {
	0.1f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.1f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.1f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};


static vec3 x_axis = {1.0f, 0.0f, 0.0f};
static vec3 y_axis = {0.0f, 1.0f, 0.0f};
static vec3 z_axis = {0.0f, 0.0f, 1.0f};


mat4 *get_current_transform_matrix(void)
{
	return &current_transform_matrix;
}


void process_input(GLFWwindow *window)
{
}
