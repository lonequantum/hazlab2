#ifndef _HAZLAB2_
#define _HAZLAB2_
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>


enum {NORTH, EAST, SOUTH, WEST};


// All needed vertex attributes
typedef struct {
	GLfloat x, y, z;
} VERTEX;


// List of ordered vertices
typedef struct {
	GLsizei v_length, i_length;
	VERTEX *vertices;
	GLuint *indices;
} VERTICES_SET;


// 2D int coordinates for matrices
typedef struct {
	int row, column;
} MATRIX_COORDINATES;


// Command line defined size
#define MIN_INPUT_SIZE      1
#define MAX_INPUT_SIZE     16
#define DEFAULT_INPUT_SIZE  8


// Command line defined MSAA
#define DEFAULT_AA_SAMPLES  2
#define MAX_AA_SAMPLES      8


// if x is the width of the maze (the matrix), divide x * x by this number to obtain the number of paths to add in case of non-perfect maze
#define NON_PERFECT_RATIO  49


// maximum number of pointers to store for deallocation
#define MAX_MEM_OBJECTS    50


// see genmaze.c
#define WALLS_THICKNESS_REDUCTOR (GLfloat)0.25


// Clipping and projection values
#define CLIP_MIN          0.0f
#define CLIP_MAX (float)(MAX_INPUT_SIZE * 4 - 1)
#define FOV_ANGLE_RAD     1.0f


// The game window size
#define WINDOW_WIDTH     1024
#define WINDOW_HEIGHT     576


// The speed of movements
#define DELTA_ROTATION            0.075
#define DELTA_TRANSLATION_DIVISOR 10.0


// The way the user looks at the environment
#define LOOK_AT_DELTA_Y (GLfloat)-0.025
#define USER_Y          (GLfloat)0.2


// Rendering values
// See also shader-level hardcoded values in render.c
#define BACKGROUND_COLOR (GLfloat)0.1, (GLfloat)0.1, (GLfloat)0.2, (GLfloat)1.0


// printf to standard error
#define eprintf(s, ...) fprintf(stderr, (s), __VA_ARGS__)


// genmaze.c
int   **generate_matrix(const int, const bool);
VERTICES_SET get_vertices(const int, int *[]);


// debug.c
void    print_matrix(int, int *[]);
void    print_key_vertices(VERTICES_SET);
void    prepare_scene_debug_adds(void);


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
bool    prepare_scene(const VERTICES_SET);
void    delete_GL_data(void);


// move.c
mat4   *get_current_transform_matrix(void);
void    process_input(GLFWwindow *, const int, const int *const []);


#endif
