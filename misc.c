#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "hazlab2.h"


// Returns whether a string represents a positive integer
bool is_positive_integer(const char *restrict s)
{
	while (*s)
		if (!isdigit(*s++))
			return false;

	return true;
}


static bool rand_initialized = false;


// Gives a random integer value between 0 and upper_bound - 1
int random_integer(const int upper_bound)
{
	if (rand_initialized == false)
	{
		srand(time(NULL));
		rand_initialized = true;
	}

	return (int)(rand() * (double)upper_bound / RAND_MAX);
}


// Flood-fills a square int matrix, starting at (from_i, from_j), replacing contiguous "target" values by "replace" values
void flood_fill(const int size, int *lines[], const int from_i, const int from_j, const int target, const int replace)
{
	MATRIX_COORDINATES *sp, *stack;
	sp = stack = allocate(size * size * sizeof(MATRIX_COORDINATES));
	MATRIX_COORDINATES mc;

	mc.row = from_i; mc.column = from_j;
	*sp++ = mc;

	while (sp != stack)
	{
		mc = *--sp;
		lines[mc.row][mc.column] = replace;

		if (mc.row > 0 && lines[mc.row - 1][mc.column] == target)
		{
			MATRIX_COORDINATES m = {.row = mc.row - 1, .column = mc.column};
			*sp++ = m;
		}
		if (mc.row < size - 1 && lines[mc.row + 1][mc.column] == target)
		{
			MATRIX_COORDINATES m = {.row = mc.row + 1, .column = mc.column};
			*sp++ = m;
		}
		if (mc.column > 0 && lines[mc.row][mc.column - 1] == target)
		{
			MATRIX_COORDINATES m = {.row = mc.row, .column = mc.column - 1};
			*sp++ = m;
		}
		if (mc.column < size - 1 && lines[mc.row][mc.column + 1] == target)
		{
			MATRIX_COORDINATES m = {.row = mc.row, .column = mc.column + 1};
			*sp++ = m;
		}
	}

	deallocate_last();
}
