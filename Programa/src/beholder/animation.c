#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include "animation.h"
#include "drawing.h"

#define TOTALTIME 300.0
#define TIMESTEP 5.0


/** El thread encargado de actualizar el contenido de la ventana */
static pthread_t* update_thread;

/** Intercambia los valores de dos variables a y b */
void double_swap(double* a, double* b)
{
	double aux = *a;
	*a = *b;
	*b = aux;
}

/** Anima cuadro por cuadro el desplazamiento de una fila o columna */
void animate_shift(GtkWidget* canvas, Content* cont, uint8_t index, char code)
{
	/* La animacion es de el movimiento de una fila? */
	bool row;
	/* Hacia que lado es el desplazamiento? (El eje Y crece hacia abajo) */
	int8_t signum;
	/* La funcion de desplazamiento correspondiente para el puzzle */
	shift_fn_t shift_fn;

	/* Todo eso está definido por la dirección del movimiento */
	switch(code)
	{
		/* Right */
		case 'R': row = true;  signum = 1;  shift_fn = puzzle_shift_right; break;
		/* Up */
		case 'U': row = false; signum = -1; shift_fn = puzzle_shift_up;    break;
		/* Left */
		case 'L': row = true;  signum = -1; shift_fn = puzzle_shift_left;  break;
		/* Down */
		case 'D': row = false; signum = 1;  shift_fn = puzzle_shift_down;  break;
		default : abort();
	}

	/* Bounding rectangle of affected area */
	uint8_t len = row ? cont -> puz -> width : cont -> puz -> height;

	double start_x = cont -> cell_size / 2 + cont -> cell_size * index;
	double start_y = 0;
	double width = cont -> cell_size;
	double height = (len + 1) * cont -> cell_size;

	if(row)
	{
		double_swap(&start_x, &start_y);
		double_swap(&width, &height);
	}

	/* Animation parameters */
	double frames = TOTALTIME / TIMESTEP;
	double delta = cont -> cell_size / frames;

	/* Animation itself */
	for(int i = 0; i < frames; i++)
	{
		if(row)
		{
			cont -> row_offset[index] += signum*delta;
		}
		else
		{
			cont -> col_offset[index] += signum*delta;
		}

		gtk_widget_queue_draw_area(canvas, start_x, start_y, width, height);
		usleep(TIMESTEP * 1000);
	}
	if(row)
	{
		cont -> row_offset[index] = 0;
	}
	else
	{
		cont -> col_offset[index] = 0;
	}

	shift_fn(cont -> puz, index);
	gtk_widget_queue_draw(canvas);
}

/** Lleva a cabo la actualización del tablero */
void* update(void* ptr)
{
	/* Desencapsula los parámetros */
	void** param = ptr;
	GtkWidget* canvas = param[0];
	Content* cont = param[1];
	free(param);

	uint8_t range[4] =
	{
		cont -> puz -> height,
		cont -> puz -> width,
		cont -> puz -> height,
		cont -> puz -> width
	};

	char codes[4] =
	{
		'R',
		'U',
		'L',
		'D'
	};




	usleep(1000 * 1000);


	while(true)
	{
		int fn = rand() % 4;
		uint8_t index = rand() % range[fn];

		animate_shift(canvas, cont, index, codes[fn]);

		usleep((TOTALTIME) * 1000);
	}




	// /* Row, Column & Value */
	// uint8_t r, c, v;
	//
	//
	//
	// char buf[8];
	//
	// while(true)
	// {
	// 	fscanf(stdin, "%s", buf);
	//
	// 	if(!strcmp(buf, "SET"))
	// 	{
	// 		fscanf(stdin, "%hhu %hhu %hhu", &r, &c, &v);
	// 		cont -> puz -> matrix[r][c] = v;
	// 		gtk_widget_queue_draw(canvas);
	// 	}
	// 	else if(!strcmp(buf, "MOV"))
	// 	{
	// 		fscanf(stdin, "%hhu", &r);
	// 		animate_shift_right(canvas, cont, r);
	// 	}
	// 	else if(!strcmp(buf, "END"))
	// 	{
	// 		break;
	// 	}
	// 	else
	// 	{
	// 		abort();
	// 	}
	// }

	pthread_exit(NULL);
}

/** Inicializa el thread que animará el programa */
void animation_init(GtkWidget* canvas, gpointer user_data)
{
	/* Encapsula ambos parámetros dentro de un mismo puntero */
	void** param = calloc(2, sizeof(void*));
	param[0] = canvas;
	param[1] = user_data;

  /* Inicializamos el thread */
  update_thread = malloc(sizeof(pthread_t));
  /* Y lo lanzamos */
  pthread_create(update_thread, NULL, update, param);
}

void animation_abort()
{
	pthread_cancel(*update_thread);
	free(update_thread);
}
