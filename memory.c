#include <stdio.h>
#include <stdlib.h>
#include "hazlab2.h"


static void *objects[MAX_MEM_OBJECTS];
static size_t n_objects;


// Wraps the use of malloc() into an easy way to manage memory
void *allocate(const size_t bytes)
{
	if (n_objects == MAX_MEM_OBJECTS)
	{
		eprintf("maximum number of memory objects hit\n", NULL);
		return NULL;
	}

	void *ptr = malloc(bytes);
	if (ptr == NULL)
		eprintf("could not allocate memory (%ld bytes)\n", bytes);
	else
		objects[n_objects++] = ptr;

	return ptr;
}


// Frees all the dynamically-allocated memory (via allocate() only)
void deallocate_all(void)
{
	while (n_objects)
		free(objects[--n_objects]);
}
