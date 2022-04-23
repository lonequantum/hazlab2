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


// Prints the (x,z) coordinates of "key" vertices on stdout
void print_key_vertices(VERTICES_SET verts)
{
	float pv2x = -1.0, pv2z; // previous values

	for (int i = 0; i < verts.i_length; i+= 6)
	{
		VERTEX v1 = verts.vertices[verts.indices[i]];
		VERTEX v2 = verts.vertices[verts.indices[i + 2]];

		if (pv2x != -1.0 && (pv2x != v1.x || pv2z != v1.z))
			putchar('\n');

		printf("(%.0f,%.0f -> %.0f,%.0f)", v1.x, v1.z, v2.x, v2.z);

		if (v1.x != v2.x && v1.z != v2.z)
			printf("algo error: vertices not aligned\n");

		pv2x = v2.x; pv2z = v2.z;
	}
	putchar('\n');
}


// Additionnal instructions for rendering
void prepare_scene_debug_adds(void)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}


#endif
