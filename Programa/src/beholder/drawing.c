#include <stdio.h>
#include "drawing.h"
#include <stdlib.h>
/** La razón entre ambos cuadrados de una celda */
#define RATIO 0.5
/** Color de fondo */
#define BGC (Color){.R = 13.5 /255.0, .G = 13.5 /255.0,.B = 23.5 /255.0}
/** Color de las flechas */
#define WTE (Color){.R = 200.0/255.0, .G = 200.0/255.0,.B = 200.0/255.0}
/** Color de las flechas apretadas */
#define PRS (Color){.R = 0.0/255.0, .G = 0.0/255.0,.B = 255.0/255.0}

/* ------------------------------ Útiles ---------------------------------- */

/** Multiplica un color por un escalar, para aclararlo o oscurecerlo */
Color color_scale(Color color, double k)
{
  return (Color){.R = color.R * k,.G = color.G * k, .B = color.B * k};
}

/** Setea el color RGB de cairo */
void color_dip(cairo_t* cr, Color color)
{
  cairo_set_source_rgb(cr,color.R,color.G, color.B);
}

/* ----------------------------- Flechas ---------------------------------- */

/** Dibuja el cuarto superior de un cuadrado */
void draw_top_arrow(cairo_t* cr, Color color, double x, double y, double size)
{
  cairo_move_to(cr, x - size/2, y - size/2);
  cairo_rel_line_to(cr, size, 0);
  cairo_line_to(cr,x,y);
  cairo_close_path(cr);
  color_dip(cr, color);
  cairo_fill(cr);
}

/** Dibuja el cuarto izquierdo de un cuadrado */
void draw_left_arrow(cairo_t* cr, Color color, double x, double y, double size)
{
  cairo_move_to(cr, x - size/2, y - size/2);
  cairo_rel_line_to(cr, 0, size);
  cairo_line_to(cr,x,y);
  cairo_close_path(cr);
  color_dip(cr, color);
  cairo_fill(cr);
}

/** Dibuja el cuarto derecho de un cuadrado */
void draw_right_arrow(cairo_t* cr, Color color, double x, double y, double size)
{
  cairo_move_to(cr, x + size/2, y - size/2);
  cairo_rel_line_to(cr, 0, size);
  cairo_line_to(cr,x,y);
  cairo_close_path(cr);
  color_dip(cr, color);
  cairo_fill(cr);
}

/** Dibuja el cuarto inferior de un cuadrado */
void draw_bottom_arrow(cairo_t* cr, Color color, double x, double y, double size)
{
  cairo_move_to(cr, x - size/2, y + size/2);
  cairo_rel_line_to(cr, size, 0);
  cairo_line_to(cr,x,y);
  cairo_close_path(cr);
  color_dip(cr, color);
  cairo_fill(cr);
}

/* ----------------------------- Bloques ---------------------------------- */


/** Dibuja un bloque centrado en x e y, del tamaño especificado */
void draw_block(cairo_t* cr, Color color, double x, double y, double size)
{
  /* Las dimensiones del cuadrado mayor, menos un poco para dejar espacio */
  double outer = 0.95 * size;
  /* Establecemos el grosor de linea */
  cairo_set_line_width(cr, outer/(MAX_DIMENSION/2));

  /* Rellena el cuadrado entero primero */
  cairo_rectangle(cr, x - outer / 2, y - outer/2, outer, outer);
  color_dip(cr,color);
  cairo_fill(cr);

  /* Top fragment */
  draw_top_arrow(cr, color_scale(color, 1.5), x, y, outer);

  /* Left Fragment */
  draw_left_arrow(cr, color_scale(color, 1.25), x, y, outer);

  /* Right Fragment */
  draw_right_arrow(cr, color_scale(color, 0.75), x, y, outer);

  /* Bottom fragment */
  draw_bottom_arrow(cr, color_scale(color, 0.5), x, y, outer);

  /* Dibujamos el cuadrado interno */
  double inner = RATIO * outer;
  cairo_rectangle(cr, x - inner/2, y - inner/2, inner, inner);
  color_dip(cr, color_scale(color, 1));
  cairo_fill(cr);
}

void draw_frame(cairo_t* cr, Content* cont)
{
	color_dip(cr, BGC);
	double sx, sy, w, h;
	sx = 0;
	sy = 0;
	w = (cont -> puz -> width + 1) * cont -> cell_size;
	h = cont -> cell_size / 2;

	cairo_rectangle(cr, sx, sy, w, h);

	sy = (cont -> puz -> height + 0.5) * cont -> cell_size;

	cairo_rectangle(cr, sx, sy, w, h);

	sy = cont -> cell_size / 2;
	w = cont -> cell_size / 2;
	h = cont -> puz -> height * cont -> cell_size;

	cairo_rectangle(cr, sx, sy, w, h);

	sx = (cont -> puz -> width + 0.5) * cont -> cell_size;

	cairo_rectangle(cr, sx, sy, w, h);


	cairo_fill(cr);
}

bool canvas_draw(cairo_t* cr, Content* cont)
{
	/* Color de fondo */
	color_dip(cr, BGC);
	cairo_paint(cr);

	uint8_t width = cont -> puz -> width;
	uint8_t height = cont -> puz -> height;

	if(cont -> mode == ALL)
	{
		/* Dibuja los bloques */
		for(int row = 0; row < height; row++)
	  {
	    for(int col = 0; col < width; col++)
	    {
				int r = (height + row) % height;
				int c = (width + col) % width;

	      double sx = cont -> cell_size;
	      double sy = cont -> cell_size;
	      double cx = sx + col * cont -> cell_size;
				double cy = sy + row * cont -> cell_size;

				Color color = cont -> color_table[cont -> puz -> matrix[r][c]];
	      draw_block(cr, color, cx, cy, cont -> cell_size);
	    }
	  }

		/* Dibuja las flechas */
		for(int col = 0; col < cont -> puz -> width; col++)
	  {
			draw_top_arrow(cr, WTE, (col+1) * cont -> cell_size, 0.4 * cont -> cell_size, cont -> cell_size/2);
			draw_bottom_arrow(cr, WTE, (col+1) * cont -> cell_size, (cont -> puz -> height + 0.6) * cont -> cell_size, cont -> cell_size/2);
	  }

		for(int row = 0; row < cont -> puz -> height; row++)
	  {
			draw_left_arrow(cr, WTE, 0.4 * cont -> cell_size, (row+1) * cont -> cell_size, cont -> cell_size / 2);
			draw_right_arrow(cr, WTE, (cont -> puz -> width + 0.6) * cont -> cell_size, (row+1) * cont -> cell_size, cont -> cell_size/2);
	  }
	}
	else if(cont -> mode == ROW)
	{
		int row = cont -> index;

		/* Dibuja los bloques */
    for(int col = -1; col <= width; col++)
    {
			int r = (height + row) % height;
			int c = (width + col) % width;

      double sx = cont -> cell_size;
      double sy = cont -> cell_size;
      double cx = sx + col * cont -> cell_size + cont -> offset;
			double cy = sy + row * cont -> cell_size;

			Color color = cont -> color_table[cont -> puz -> matrix[r][c]];
      draw_block(cr, color, cx, cy, cont -> cell_size);
    }

		/* Tapa los bloques exteriores */
		draw_frame(cr, cont);

		/* Dibuja las flechas */
		Color lc = cont -> offset > 0 ? PRS : WTE;
		Color rc = cont -> offset < 0 ? PRS : WTE;

		draw_left_arrow(cr, lc, 0.4 * cont -> cell_size, (row+1) * cont -> cell_size, cont -> cell_size/2);
		draw_right_arrow(cr, rc, (cont -> puz -> width + 0.6) * cont -> cell_size, (row+1) * cont -> cell_size, cont -> cell_size/2);
	}
	else if(cont -> mode == COL)
	{
		int col = cont -> index;

		/* Dibuja los bloques */
		for(int row = -1; row <= height; row++)
	  {
			int r = (height + row) % height;
			int c = (width + col) % width;

      double sx = cont -> cell_size;
      double sy = cont -> cell_size;
      double cx = sx + col * cont -> cell_size;
			double cy = sy + row * cont -> cell_size + cont -> offset;

			Color color = cont -> color_table[cont -> puz -> matrix[r][c]];
      draw_block(cr, color, cx, cy, cont -> cell_size);
	  }

		/* Tapa los bloques exteriores */
		draw_frame(cr, cont);

		/* Dibuja las flechas */

		Color tc = cont -> offset > 0 ? PRS : WTE;
		Color bc = cont -> offset < 0 ? PRS : WTE;

		draw_top_arrow(cr, tc, (col+1) * cont -> cell_size, 0.4 * cont -> cell_size, cont -> cell_size/2);
		draw_bottom_arrow(cr, bc, (col+1) * cont -> cell_size, (cont -> puz -> height + 0.6) * cont -> cell_size, cont -> cell_size/2);
	}
	else
	{
		abort();
	}

	return true;
}
