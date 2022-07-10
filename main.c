#include <glad/glad.h>
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
static GLFWwindow *start_graphics(unsigned int antialias_samples)
{
	if (!glfwInit())
		return NULL;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, antialias_samples);
	/*glEnable(GL_MULTISAMPLE);*/

	glfwSetErrorCallback(on_glfw_error);

	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hazlab2", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		return NULL;

	glfwSetWindowSizeLimits(window, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	return window;
}


// Closes the game window and destroys the context
static void end_graphics(GLFWwindow *const window)
{
	delete_GL_data();
	glfwDestroyWindow(window);
	glfwTerminate();
}


// Parses the command line and launches the game
int main(int argc, char *argv[])
{
	atexit(deallocate_all);

	unsigned int size = DEFAULT_INPUT_SIZE;
	unsigned int antialias_samples = DEFAULT_AA_SAMPLES;
	bool perfect = false;

	int opt;
	while ((opt = getopt(argc, argv, ":ps:a:")) != -1)
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

		case 'a':
			if (is_positive_integer(optarg))
			{
				antialias_samples = atoi(optarg);
				if (antialias_samples > MAX_AA_SAMPLES)
				{
					eprintf("antialiasing samples operand (option -a) must be <= %d\n", MAX_AA_SAMPLES);
					return EX_USAGE;
				}
			}
			else
			{
				eprintf("option -a (antialiasing samples) requires a positive integer operand\n", NULL);
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
	VERTICES_SET data = get_vertices(size, matrix);
#ifndef NDEBUG
	//print_key_vertices(data);
	print_matrix(size, matrix);
#endif

	// Creates the window
	GLFWwindow *window = start_graphics(antialias_samples);
	if (!window)
		return EXIT_FAILURE;

	// Initializes graphic data
	if (!prepare_scene(data))
		return EXIT_FAILURE;

	// Configures and runs the main loop
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(window))
	{
		draw_scene();
		glfwSwapBuffers(window);

		process_input(window, size, (const int **)matrix);
		glfwPollEvents();
	}

	// Exits properly
	end_graphics(window);

	return EXIT_SUCCESS;
}
