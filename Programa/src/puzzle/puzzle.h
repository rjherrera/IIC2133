#pragma once

#include <stdint.h>
#include <stdbool.h>

/** Representa un puzzle de colores */
struct puzzle
{
	/** La matriz de colores, cada uno un número entre 0 y 7 inclusive */
	uint8_t** matrix;
	/** Alto de la matriz*/
	uint8_t height;
	/** Ancho de la matriz */
	uint8_t width;
};

/** Representa un puzzle de colores */
typedef struct puzzle Puzzle;

/** Lee un puzzle a partir de un stream */
Puzzle* puzzle_from_stream (void* stream);
/** Desplaza la fila especificada hacia la izquierda */
void    puzzle_shift_left  (Puzzle* puzzle, uint8_t row);
/** Desplaza la fila especificada hacia la derecha */
void    puzzle_shift_right (Puzzle* puzzle, uint8_t row);
/** Desplaza la columna especificada hacia arriba */
void    puzzle_shift_up    (Puzzle* puzzle, uint8_t col);
/** Desplaza la columna especificada hacia abajo */
void    puzzle_shift_down  (Puzzle* puzzle, uint8_t col);
/** Imprime el puzzle a un stream determinado */
int     puzzle_print       (Puzzle* puzzle, void* stream);

/** Funciones de desplazamiento */
typedef void (*shift_fn_t)(Puzzle*,uint8_t);

/****************** Funciones útiles para el diccionario ********************/

/** Indica si dos puzzles son equivalentes */
bool    puzzle_equals      (Puzzle* a, Puzzle* b);
/** Crea una copia exacta del puzzle */
Puzzle* puzzle_clone       (Puzzle* puzzle);
/** Libera los recursos asociados a un  puzzle */
void    puzzle_destroy     (Puzzle* puzzle);
