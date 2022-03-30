#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "hazlab2.h"


static bool make_path(const int size, int *lines[], const int from_i, const int from_j, const int id)
{
	lines[from_i][from_j] = id;

	bool directions[4];
	directions[0] = directions[1] = directions[2] = directions[3] = true;
	int direction = random_integer(4);
	int i, j, tries = 0;
	do
	{
		direction++;
		if (direction == 4)
			direction = 0;
		i = from_i, j = from_j;
		switch (direction)
		{
		case 0: i-= 2; break;
		case 1: j+= 2; break;
		case 2: i+= 2; break;
		case 3: j-= 2;
		}
		if (i == -1 || i == size || j == -1 || j == size || lines[i][j] == id)
			directions[direction] = false;
	} while (directions[direction] == false && ++tries < 4);

	if (tries == 4)
		return false;

	switch (direction)
	{
	case 0: lines[from_i - 1][from_j] = 1; break;
	case 1: lines[from_i][from_j + 1] = 1; break;
	case 2: lines[from_i + 1][from_j] = 1; break;
	case 3: lines[from_i][from_j - 1] = 1;
	}

	if (lines[i][j] == 0)
		return make_path(size, lines, i, j, id);

	return true;
}


int **generate_matrix(const int size, const bool perfect)
{
	int **lines = allocate(size * sizeof(int*));
	if (lines == NULL)
		return NULL;

	int *space = allocate(size * size * sizeof(int));
	if (space == NULL)
		return NULL;
	memset(space, 0, size * size * sizeof(int));

	for (int s = 0; s < size; s++)
		lines[s] = space + s * size;

	int path_id = 0;
	for (int i = 1; i < size; i+= 2)
		for (int j = 1; j < size; j+= 2)
			if (lines[i][j] == 0)
			{
				int tries = 0;
				path_id++;
				while (make_path(size, lines, i, j, path_id) == false && ++tries < 4);
			}

	int exit = random_integer(size / 2) * 2 + 1;
	switch (random_integer(4))
	{
	case 0: lines[   0    ][exit] = 1; break;
	case 1: lines[size - 1][exit] = 1; break;
	case 2: lines[exit][   0    ] = 1; break;
	case 3: lines[exit][size - 1] = 1;
	}

	if (perfect == false)
	{
		int max_additionnal_paths = size * size / NON_PERFECT_RATIO;
		for (int n = 0; n < max_additionnal_paths; n++)
		{
			int i = random_integer(size / 2) * 2 + 1;
			int j = random_integer(size / 2 - 1) * 2 + 2;
			lines[i][j] = 1;
		}
	}

	return lines;
}
