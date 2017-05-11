#pragma once

#include <cairo.h>
#include <stdbool.h>
#include "color.h"
#include "../puzzle/puzzle.h"

#define CELL_SIZE 64.0
#define MAX_DIMENSION 512.0

/** Indica el modo de dibujo */
enum draw_mode
{
	/** Dibuja una fila */
	ROW,
	/** Dibuja una columna */
	COL,
	/** Dibuja el tablero entero */
	ALL
};

/** Indica el modo de dibujo */
typedef enum draw_mode Mode;



/** Contiene la información gráfica del puzzle */
struct content
{
	/** El puzzle del canvas */
	Puzzle* puz;
	/** Tamaño de una celda */
	double cell_size;
	/** La tabla con los distintos colores */
	Color* color_table;
	/** El desplazamiento que tiene una fila hacia la derecha */
	double* row_offset;
	/** El desplazamiento que tiene una columna hacia abajo */
	double* col_offset;
};
/** Contiene la información de las dimensiones del canvas */
typedef struct content Content;

/** Dibuja sobre el canvas dado */
bool canvas_draw(cairo_t* cr, Content* cont);
