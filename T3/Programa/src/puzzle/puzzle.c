#include "puzzle.h"
#include <stdlib.h>
#include <stdio.h>

/** Lee un puzzle a partir de un stream */
Puzzle* puzzle_from_stream(void* stream)
{
	Puzzle* puz = malloc(sizeof(Puzzle));

	fscanf(stream, "%hhu", &puz -> height) ? : abort();
	fscanf(stream, "%hhu", &puz -> width) ? : abort();

	puz -> matrix = calloc(puz -> height, sizeof(uint8_t*));
	for(uint8_t row = 0; row < puz -> height; row++)
	{
		puz -> matrix[row] = calloc(puz -> width, sizeof(uint8_t));
		for(uint8_t col = 0; col < puz -> width; col++)
		{
			fscanf(stream, "%hhu", &puz -> matrix[row][col]) ? : abort();
		}
	}
	return puz;
}

/** Indica si dos puzzles son equivalentes */
bool puzzle_equals(Puzzle* a, Puzzle* b)
{
	if(a -> height != b -> height) return false;
	if(a -> width != b -> width) return false;

	for(uint8_t row = 0; row < a -> height; row++)
	{
		for(uint8_t col = 0; col < a -> width; col++)
		{
			if(a -> matrix[row][col] != b -> matrix[row][col])
			{
				return false;
			}
		}
	}
	return true;
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

/** Desliza la fila especificada hacia la izquierda */
void puzzle_shift_left(Puzzle* puz, uint8_t row)
{
	uint8_t aux = puz -> matrix[row][0];
	for(uint8_t col = 0; col < puz -> width - 1; col++)
	{
		puz -> matrix[row][col] = puz -> matrix[row][col + 1];
	}
	puz -> matrix[row][puz -> width - 1] = aux;
}

/** Desplaza la fila especificada hacia la derecha */
void puzzle_shift_right(Puzzle* puz, uint8_t row)
{
	uint8_t aux = puz -> matrix[row][puz -> width - 1];
	for(uint8_t col = puz -> width - 1; col > 0; col--)
	{
		puz -> matrix[row][col] = puz -> matrix[row][col - 1];
	}
	puz -> matrix[row][0] = aux;
}

/** Desplaza la columna especificada hacia arriba */
void puzzle_shift_up(Puzzle* puz, uint8_t col)
{
	uint8_t aux = puz -> matrix[0][col];
	for(uint8_t row = 0; row < puz -> height - 1; row++)
	{
		puz -> matrix[row][col] = puz -> matrix[row + 1][col];
	}
	puz -> matrix[puz -> height - 1][col] = aux;
}

/** Desplaza la columna especificada hacia abajo */
void puzzle_shift_down(Puzzle* puz, uint8_t col)
{
	uint8_t aux = puz -> matrix[puz -> height - 1][col];
	for(uint8_t row = puz -> height - 1; row > 0; row--)
	{
		puz -> matrix[row][col] = puz -> matrix[row - 1][col];
	}
	puz -> matrix[0][col] = aux;
}

/** Imprime el puzzle a un stream determinado */
int puzzle_print(Puzzle* puz, void* stream)
{
	if(fprintf(stream, "%hhu\n", puz -> height) < 0)
	{
		return -1;
	}
	if(fprintf(stream, "%hhu\n", puz -> width) < 0)
	{
		return -1;
	}
	for(uint8_t row = 0; row < puz -> height; row++)
	{
		for(uint8_t col = 0; col < puz -> width; col++)
		{
			if(fprintf(stream, "%hhu ", puz -> matrix[row][col]) < 0)
			{
				return -1;
			}
		}
		if(fprintf(stream, "\n") < 0)
		{
			return -1;
		}
	}
	return 1;
}

/** Crea una copia exacta del puzzle */
Puzzle* puzzle_clone(Puzzle* old)
{
	Puzzle* new = malloc(sizeof(Puzzle));

	new -> height = old -> height;
	new -> width = old -> width;

	new -> matrix = calloc(new -> height, sizeof(uint8_t*));
	for(uint8_t row = 0; row < new -> height; row++)
	{
		new -> matrix[row] = calloc(new -> width, sizeof(uint8_t));
		for(uint8_t col = 0; col < new -> width; col++)
		{
			new -> matrix[row][col] = old -> matrix[row][col];
		}
	}
	return new;
}
