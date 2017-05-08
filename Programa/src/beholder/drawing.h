#pragma once

#include <cairo.h>
#include <stdbool.h>
#include "color.h"
#include "../puzzle/puzzle.h"

#define CELL_SIZE 64.0
#define MAX_DIMENSION 512.0

/** Contiene la información gráfica del puzzle */
struct content
{
	/** El puzzle del canvas */
	Puzzle* puz;
	/** Tamaño de una celda */
	double cell_size;
	/** La tabla con los distintos colores */
	Color* color_table;
};
/** Contiene la información de las dimensiones del canvas */
typedef struct content Content;

/** Dibuja sobre el canvas dado */
bool canvas_draw(cairo_t* cr, Content* cont);
