#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../puzzle/puzzle.h"
#include "window.h"
#include "drawing.h"
#include "color.h"

/** Lee el estado inicial del puzzle a partir de la consola */
static Puzzle* initial_state()
{
	Puzzle* puz = malloc(sizeof(Puzzle));

	puz -> height = 4;
	puz -> width = 5;

	// fscanf(stdin, "%hhu", &puz -> height);
	// fscanf(stdin, "%hhu", &puz -> width);

	puz -> matrix = calloc(puz -> height, sizeof(uint8_t*));
	for(uint8_t row = 0; row < puz -> height; row++)
	{
		puz -> matrix[row] = calloc(puz -> width, sizeof(uint8_t));
		for(uint8_t col = 0; col < puz -> width; col++)
		{
			puz -> matrix[row][col] = (row+col) % 8;
			// fscanf(stdin, "%hhu", &puz -> matrix[row][col]);
		}
	}
	return puz;
}

static inline bool check_parameters(int argc, char** argv)
{
	return argc == 1;
}

int main(int argc, char** argv)
{
	/* Se revisa que los parametros entregados sean correctos */
  if(!check_parameters(argc, argv)) return 1;

	Content cont;
	cont.color_table = color_table();
	cont.puz = initial_state();
	cont.mode = ALL;
	cont.offset = 0;
	cont.index = 0;

	window_create(&cont);

	puzzle_destroy(cont.puz);
	free(cont.color_table);

	return 0;
}
