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

/** Representa un sector rectangular de la ventana */
struct rectarea
{
	/** Componente X de la esquina superior izquierda de esta área */
	double sx;
	/** Componente Y de la esquina superior izquierda de esta área */
	double sy;
	/** Ancho de esta área */
	double width;
	/** Alto de esta área */
	double height;
};

/** Representa un sector rectangular de la ventana */
typedef struct rectarea Area;

Area get_draw_area(Content* cont, bool row, uint8_t index, int8_t signum)
{
	Area a;

	if(row)
	{
		a.sy = cont -> cell_size / 2 + cont -> cell_size * index;
		a.height = cont -> cell_size;
		a.width = (cont -> puz -> width + 0.5) * cont -> cell_size;

		if(signum > 0)
		{
			/* Shift Right */
			a.sx = 0;
		}
		else
		{
			/* Shift Left */
			a.sx = cont -> cell_size / 2;
		}
	}
	else
	{
		a.sx = cont -> cell_size / 2 + cont -> cell_size * index;
		a.width = cont -> cell_size;
		a.height = (cont -> puz -> height + 0.5) * cont -> cell_size;

		if(signum > 0)
		{
			/* Shift down */
			a.sy = 0;
		}
		else
		{
			/* Shift up */
			a.sy = cont -> cell_size / 2;
		}
	}


	return a;
}

void animate_shift(GtkWidget* canvas, Content* cont, bool row, uint8_t index, int8_t signum, shift_fn_t shift_fn)
{
	double frames = TOTALTIME / TIMESTEP;
	double delta = cont -> cell_size / frames;

	Area a = get_draw_area(cont, row, index, signum);

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

		gtk_widget_queue_draw_area(canvas,a.sx,a.sy,a.width,a.height);
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
	// gtk_widget_queue_draw_area(canvas,a.sx,a.sy,a.width,a.height);
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

	shift_fn_t fns[4] =
	{
		puzzle_shift_right,
		puzzle_shift_left,
		puzzle_shift_up,
		puzzle_shift_down
	};

	int8_t signums[4] =
	{
		1,
		-1,
		-1,
		1
	};

	bool rowicity[4] =
	{
		true,
		true,
		false,
		false
	};

	uint8_t range[4] =
	{
		cont -> puz -> height,
		cont -> puz -> height,
		cont -> puz -> width,
		cont -> puz -> width
	};




	usleep(1000 * 1000);


	while(true)
	{



		int fn = rand() % 4;
		uint8_t index = rand() % range[fn];

		animate_shift(canvas, cont, rowicity[fn], index, signums[fn], fns[fn]);

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
