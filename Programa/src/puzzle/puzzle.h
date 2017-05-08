#pragma once

#include <stdint.h>

/** Representa un puzzle de colores */
struct puzzle
{
	/** La matriz de colores, cada uno un número entre 0 y 7 */
	uint8_t** matrix;
	/** Ancho de la matriz */
	uint8_t width;
	/** Alto de la matriz*/
	uint8_t height;
};

/** Representa un puzzle de colores */
typedef struct puzzle Puzzle;

/** Lee un puzzle a partir de un archivo */
Puzzle* puzzle_from_file (char* path);
/** Libera los recursos asociados a un  puzzle */
void    puzzle_destroy   (Puzzle* puzzle);
