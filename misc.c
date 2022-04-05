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
