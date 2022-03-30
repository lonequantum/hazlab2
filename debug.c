#ifndef NDEBUG

#include <stdio.h>
#include "hazlab2.h"


void print_matrix(int size, int *lines[])
{
	for (int line = 0; line < size; line++)
	{
		for (int column = 0; column < size; column++)
			if (line == size / 2 && column == size / 2)
				printf("><");
			else if (lines[line][column])
				printf("  ");
			else
				printf("██");
		putchar('\n');
	}
}

#endif
