#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "hazlab2.h"


bool is_positive_integer(const char *s)
{
	while (*s) if (!isdigit(*s++)) return false;

	return true;
}


static bool rand_initialized = false;


int random_integer(const int upper_bound)
{
	if (rand_initialized == false)
	{
		srand(time(NULL));
		rand_initialized = true;
	}

	return (int)(rand() * (double)upper_bound / RAND_MAX);
}
