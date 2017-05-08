#include "puzzle.h"
#include <stdlib.h>

/** Lee un puzzle a partir de un archivo */
Puzzle* puzzle_from_file (char* path)
{
	abort();
}

/** Libera los recursos asociados a un  puzzle */
void puzzle_destroy(Puzzle* puzzle)
{
	for(uint8_t row = 0; row < puzzle -> height; row++)
	{
		free(puzzle -> matrix[row]);
	}
	free(puzzle -> matrix);
	free(puzzle);
}
