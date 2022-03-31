#ifndef _HAZLAB2_
#define _HAZLAB2_
#include <stdbool.h>


// Command line defined size
#define MIN_INPUT_SIZE      1
#define MAX_INPUT_SIZE     16
#define DEFAULT_INPUT_SIZE  8


// if x is the width of the maze (the matrix), divide x * x by this number to obtain the number of paths to add in case of non-perfect maze
#define NON_PERFECT_RATIO  49


// maximum number of pointers to store for deallocation
#define MAX_MEM_OBJECTS    50


// printf to standard error
#define eprintf(s, ...) fprintf (stderr, (s), __VA_ARGS__)


// genmaze.c
int   **generate_matrix(const int, const bool);


// debug.c
void    print_matrix(int, int *[]);


// memory.c
void   *allocate(const size_t);
void    deallocate_all(void);


// misc.c
bool    is_positive_integer(const char *);
int     random_integer(const int);


#endif
