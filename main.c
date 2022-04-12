#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>
#include "hazlab2.h"


// Defines the GLFW error callback
void on_glfw_error(int error, const char *description)
{
	eprintf("(glfw error %d) %s\n", error, description);
}


// Initializes the context and creates the game window
static GLFWwindow *start_graphics(void)
{
	if (!glfwInit())
		return NULL;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwSetErrorCallback(on_glfw_error);

	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hazlab2", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return NULL;
	}

	glfwSetWindowSizeLimits(window, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	glfwMakeContextCurrent(window);

	return window;
}


// Closes the game window and destroys the context
static void end_graphics(GLFWwindow *const window)
{
	glfwDestroyWindow(window);
	glfwTerminate();
}


// Parses the command line and launches the game
int main(int argc, char *argv[])
{
	atexit(deallocate_all);

	int size = DEFAULT_INPUT_SIZE;
	bool perfect = false;

	int opt;
	while ((opt = getopt(argc, argv, ":ps:")) != -1)
	{
		switch (opt)
		{
		case 's':
			if (is_positive_integer(optarg))
			{
				size = atoi(optarg);
				if (size < MIN_INPUT_SIZE || size > MAX_INPUT_SIZE)
				{
					eprintf("size operand (option -s) must be between %d and %d\n", MIN_INPUT_SIZE, MAX_INPUT_SIZE);
					return EX_USAGE;
				}
			}
			else
			{
				eprintf("option -s (size) requires a positive integer operand\n", NULL);
				return EX_USAGE;
			}
			break;

		case 'p':
			perfect = true;
			break;

		case ':':
			eprintf("option -%c requires an operand\n", optopt);
			return EX_USAGE;

		case '?':
			eprintf("unrecognized option: -%c\n", optopt);
			return EX_USAGE;
		}
	}

	size = size * 4 - 1; // real matrix size

	// Creates the maze
	int **matrix = generate_matrix(size, perfect);
	if (!matrix)
		return EXIT_FAILURE;
	get_vertices(size, matrix);
#ifndef NDEBUG
	print_matrix(size, matrix);
#endif

	// Creates the window
	GLFWwindow *window = start_graphics();
	if (!window)
		return EXIT_FAILURE;

	// Runs the main loop
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(window))
	{
		draw_scene();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Exits properly
	end_graphics(window);

	return EXIT_SUCCESS;
}
