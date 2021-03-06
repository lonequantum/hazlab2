#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hazlab2.h"


// Defines a random series of contiguous paths in the given matrix
// Returns false if the last defined path cannot join a previous series
static bool make_path(const int size, int *const lines[], const int from_i, const int from_j, const int id)
{
	lines[from_i][from_j] = id;

	bool directions[4];
	directions[NORTH] = directions[EAST] = directions[SOUTH] = directions[WEST] = true;
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
		case NORTH:
			i-= 2; break;
		case EAST:
			j+= 2; break;
		case SOUTH:
			i+= 2; break;
		case WEST:
			j-= 2;
		}
		if (i == -1 || i == size || j == -1 || j == size || lines[i][j] == id)
			directions[direction] = false;
	} while (directions[direction] == false && ++tries < 4);

	if (tries == 4)
		return false;

	switch (direction)
	{
	case NORTH:
		lines[from_i - 1][from_j] = 1; break;
	case EAST:
		lines[from_i][from_j + 1] = 1; break;
	case SOUTH:
		lines[from_i + 1][from_j] = 1; break;
	case WEST:
		lines[from_i][from_j - 1] = 1;
	}

	if (lines[i][j] == 0)
		return make_path(size, lines, i, j, id);

	return true;
}


// Creates a new random square matrix of integers, also refered to as an "array of lines"
// Walls == 0
// Paths == 1
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

	// Makes a perfect (no-loop) maze by "digging" paths in the matrix
	int path_id = 0;
	for (int i = 1; i < size; i+= 2)
		for (int j = 1; j < size; j+= 2)
			if (lines[i][j] == 0)
			{
				int tries = 0;
				path_id++;
				while (make_path(size, lines, i, j, path_id) == false && ++tries < 4);
			}

	// Makes all paths have the same value
	for (int i = size - 1; i; i--)
		for (int j = size - 1; j; j--)
			if (lines[i][j])
				lines[i][j] = 1;

	// Ramdomly places an exit on one of the edges
	int exit = random_integer(size / 2) * 2 + 1;
	switch (random_integer(4))
	{
	case NORTH:
		lines[   0    ][exit] = 1; break;
	case EAST:
		lines[size - 1][exit] = 1; break;
	case SOUTH:
		lines[exit][   0    ] = 1; break;
	case WEST:
		lines[exit][size - 1] = 1;
	}

	// If requested (default behavior), adds paths so the maze has loops
	if (perfect == false)
	{
		int max_additionnal_paths = size * size / NON_PERFECT_RATIO;
		while (max_additionnal_paths--)
		{
			int i = random_integer(size / 2) * 2 + 1;
			int j = random_integer(size / 2 - 1) * 2 + 2;
			lines[i][j] = 1;
		}
	}

	return lines;
}


// Returns a set of ordered vertices that models the walls of the given maze-matrix (2 triangles per rectangle surface)
// Also models the ground (4 vertices, 6 indices)
// It needs to modify, temporarily, the matrix itself
VERTICES_SET get_vertices(const int size, int *lines[])
{
	// Checks the number of needed vertices (walls only)
	GLsizei n_vertices = 0;
	for (int i = 0; i < size; i+= 2)
		for (int j = 0; j < size; j+= 2)
		{
			int n = i            ? lines[i - 1][j] : 1;
			int s = i < size - 1 ? lines[i + 1][j] : 1;
			int w = j            ? lines[i][j - 1] : 1;
			int e = j < size - 1 ? lines[i][j + 1] : 1;
			if (n == w) n_vertices++;
			if (s == w) n_vertices++;
			if (n == e) n_vertices++;
			if (s == e) n_vertices++;
		}
#ifndef NDEBUG
	printf("angles: %d\n", n_vertices);
#endif
	n_vertices*= 2; // 2 vertices per angle

	VERTEX vtx, *vertices = allocate((n_vertices + 4) * sizeof(VERTEX));

	// Ground coordinates
	vtx.x = -size / (GLfloat)2.0; vtx.y = (GLfloat)-0.5; vtx.z = -size / (GLfloat)2.0; //SW
	vertices[0] = vtx;
	vtx.x =  size / (GLfloat)2.0; vtx.y = (GLfloat)-0.5; vtx.z = -size / (GLfloat)2.0; //SE
	vertices[1] = vtx;
	vtx.x = -size / (GLfloat)2.0; vtx.y = (GLfloat)-0.5; vtx.z =  size / (GLfloat)2.0; //NW
	vertices[2] = vtx;
	vtx.x =  size / (GLfloat)2.0; vtx.y = (GLfloat)-0.5; vtx.z =  size / (GLfloat)2.0; //NE
	vertices[3] = vtx;

	GLsizei v_index = 4;

	GLuint *indices = allocate((3 * n_vertices + 6) * sizeof(GLuint)); // each vertex will be used for 3 triangles, + 6 indices for the ground (2 triangles)
	GLuint *i_ptr = indices;

	// Computes the final vertices for contiguous walls
	for (int i = 0; i < size; i+= 2)
		for (int j = 0; j < size; j+= 2)
			if (lines[i][j] == 0)
			{
				int u = i, v = j;
				VERTEX vtx = {.x = (GLfloat)j, .y = (GLfloat)-0.5, .z = (GLfloat)i};
				int direction = SOUTH;
				GLsizei v_index_begin = v_index;

				do {
					if (v_index - v_index_begin)
					{
						// "closes" the first triangle of the current surface
						*i_ptr++ = v_index;

						// Defines the second triangle
						*i_ptr++ = v_index;
						*i_ptr++ = v_index - 1;
						*i_ptr++ = v_index + 1;
					}

					// Reduces the thickness of the walls (so the camera cannot get too close to them)
					vtx.x-= (((int)vtx.x % 2) * 2 - 1) * WALLS_THICKNESS_REDUCTOR;
					vtx.z-= (((int)vtx.z % 2) * 2 - 1) * WALLS_THICKNESS_REDUCTOR;

					// Makes (x, z)==(0, 0) the center of the maze
					vtx.x-= size / (GLfloat)2.0;
					vtx.z-= size / (GLfloat)2.0;

					// Starts to define the first triangle
					*i_ptr++ = v_index;
					vertices[v_index++] = vtx;            // stores the "key" vertex (y == -0.5)
					*i_ptr++ = v_index;
					vertices[v_index] = vtx;
					vertices[v_index++].y = (GLfloat)0.5; // stores the corresponding vertex (y == 0.5)

					switch (direction)
					{
					case NORTH:
						while (--u >= 0 && lines[u][v] == 0 && (v == size - 1 || lines[u][v + 1] > 0));
						vtx.x = v + 1;
						vtx.z = u + 1;
						if (u == -1 || lines[u][v])
						{
							direction = WEST;
							u++;
						}
						else
							direction = EAST;
						break;
					case EAST:
						while (++v < size && lines[u][v] == 0 && (u == size - 1 || lines[u + 1][v] > 0));
						vtx.x = v;
						vtx.z = u + 1;
						if (v == size || lines[u][v])
						{
							direction = NORTH;
							v--;
						}
						else
							direction = SOUTH;
						break;
					case SOUTH:
						while (++u < size && lines[u][v] == 0 && (v == 0 || lines[u][v - 1] > 0));
						vtx.x = v;
						vtx.z = u;
						if (u == size || lines[u][v])
						{
							direction = EAST;
							u--;
						}
						else
							direction = WEST;
						break;
					case WEST:
						while (--v >= 0 && lines[u][v] == 0 && (u == 0 || lines[u - 1][v] > 0));
						vtx.x = v + 1;
						vtx.z = u;
						if (v == -1 || lines[u][v])
						{
							direction = SOUTH;
							v++;
						}
						else
							direction = NORTH;
					}
				} while (vtx.x != j || vtx.z != i);

				// "closes" the first triangle of the last surface of the current maze part
				// (it loops to the first vertex)
				*i_ptr++ = v_index_begin;

				// Defines the last triangle
				*i_ptr++ = v_index_begin;
				*i_ptr++ = v_index - 1;
				*i_ptr++ = v_index_begin + 1;

				// marks this part of the maze as already processed
				flood_fill(size, lines, i, j, 0, -1);
			}

	// Ground indices
	*i_ptr++ = 0;
	*i_ptr++ = 1;
	*i_ptr++ = 2;
	*i_ptr++ = 2;
	*i_ptr++ = 1;
	*i_ptr++ = 3;

	// Reverts the matrix to its previous state
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			if (lines[i][j] == -1)
				lines[i][j] = 0;

	VERTICES_SET ret = {
		.v_length = n_vertices + 4,
		.i_length = 3 * n_vertices + 6,
		.vertices = vertices,
		.indices = indices
	};
	return ret;
}
