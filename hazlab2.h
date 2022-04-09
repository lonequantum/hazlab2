#ifndef _HAZLAB2_
#define _HAZLAB2_
#include <GLFW/glfw3.h>
#include <stdbool.h>


 // All needed vertex attributes
typedef struct {
	GLfloat x, y, z;
} VERTEX;


// List of ordered vertices
typedef struct {
	unsigned int v_length, i_length;
	VERTEX *vertices;
	unsigned int *indices;
} VERTICES_SET;


// 2D int coordinates for matrices
typedef struct {
	int row, column;
} MATRIX_COORDINATES;


// Command line defined size
#define MIN_INPUT_SIZE      1
#define MAX_INPUT_SIZE     16
#define DEFAULT_INPUT_SIZE  8


// if x is the width of the maze (the matrix), divide x * x by this number to obtain the number of paths to add in case of non-perfect maze
#define NON_PERFECT_RATIO  49


// maximum number of pointers to store for deallocation
#define MAX_MEM_OBJECTS    50


// The game window size
#define WINDOW_WIDTH     1024
#define WINDOW_HEIGHT     576


// printf to standard error
#define eprintf(s, ...) fprintf (stderr, (s), __VA_ARGS__)


// genmaze.c
int   **generate_matrix(const int, const bool);
VERTICES_SET get_vertices(const int, int *[]);


// debug.c
void    print_matrix(int, int *[]);


// memory.c
void   *allocate(const size_t);
void    deallocate_all(void);
void    deallocate_last(void);


// misc.c
bool    is_positive_integer(const char *);
int     random_integer(const int);
void    flood_fill(const int, int *[], const int, const int, const int, const int);


// render.c
void    draw_scene(void);


#endif
