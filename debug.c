#ifndef NDEBUG
#include <stdio.h>
#include "hazlab2.h"


// Prints a 2D view of the generated maze on standard output
void print_matrix(int size, int *lines[])
{
	for (int line = 0; line < size; line++)
	{
		for (int column = 0; column < size; column++)
			if (line == size / 2 && column == size / 2)
				printf("><");
			else if (line % 2 == 0 && column % 2 == 0
			|| (line == 0 || line == size - 1 || column == 0 || column == size - 1) && lines[line][column] == 0)
				printf("▒▒");
			else if (lines[line][column])
				printf("  ");
			else
				printf("██");
		putchar('\n');
	}
}


#endif
