#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>
#include "hazlab2.h"


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

	int **matrix = generate_matrix(size, perfect);
	if (matrix == NULL)
		return EXIT_FAILURE;

#ifndef NDEBUG
	print_matrix(size, matrix);
#endif
	return EXIT_SUCCESS;
}
