#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include "animation.h"
#include "../drawing/drawing.h"

#define TOTALTIME 192.0
#define TIMESTEP 4.0

/** El thread encargado de actualizar el contenido de la ventana */
static pthread_t* update_thread;

/** Intercambia los valores de dos variables a y b */
static void double_swap(double* a, double* b)
{
	double aux = *a;
	*a = *b;
	*b = aux;
}

/** Anima cuadro por cuadro el desplazamiento de una fila o columna */
static void animate_shift(GtkWidget* canvas, Content* cont, uint8_t index, char code)
{
	/* Hacia que lado es el desplazamiento? (El eje Y crece hacia abajo) */
	int8_t signum;
	/* La función de desplazamiento correspondiente para el puzzle */
	shift_fn_t shift_fn;

	/* Todo eso está definido por la dirección del movimiento */
	switch(code)
	{
		/* Right */
		case 'R': cont -> mode = ROW; signum = 1;  shift_fn = puzzle_shift_right; break;
		/* Up */
		case 'U': cont -> mode = COL; signum = -1; shift_fn = puzzle_shift_up;    break;
		/* Left */
		case 'L': cont -> mode = ROW; signum = -1; shift_fn = puzzle_shift_left;  break;
		/* Down */
		case 'D': cont -> mode = COL; signum = 1;  shift_fn = puzzle_shift_down;  break;
		default : abort();
	}

	/* Rectangulo que encierra el área afectada por la animación */
	uint8_t len = cont -> mode == ROW ? cont -> puz -> width : cont -> puz -> height;

	double start_x = cont -> cell_size / 2 + cont -> cell_size * index;
	double start_y = 0;
	double width = cont -> cell_size;
	double height = (len + 1) * cont -> cell_size;

	/* Las filas y columnas son recíprocas */
	if(cont -> mode == ROW)
	{
		double_swap(&start_x, &start_y);
		double_swap(&width, &height);
	}

	/* Parámetros de la animación */
	double frames = TOTALTIME / TIMESTEP;
	double delta = cont -> cell_size / frames;
	cont -> index = index;

	/* La animación en sí */
	for(int i = 0; i < frames; i++)
	{
		pthread_mutex_lock(&drawing_mutex);
			cont -> offset += signum * delta;
		pthread_mutex_unlock(&drawing_mutex);

		gtk_widget_queue_draw_area(canvas, start_x, start_y, width, height);

		usleep(TIMESTEP * 1000);
	}

	/* Hacer permantente el cambio de posición */
	pthread_mutex_lock(&drawing_mutex);
		cont -> offset = 0;
		shift_fn(cont -> puz, index);
	pthread_mutex_unlock(&drawing_mutex);

	gtk_widget_queue_draw_area(canvas, start_x, start_y, width, height);

	usleep(TOTALTIME * 1000);

	/* Resetear los parámetros */
	cont -> mode = ALL;
}

/** Lleva a cabo la actualización del tablero */
static void* update(void* ptr)
{
	/* Desencapsula los parámetros */
	void** param = ptr;
	GtkWidget* canvas = param[0];
	Content* cont = param[1];
	free(param);

	sleep(1);

	char buf[32];
	uint8_t index;

	while(true)
	{
		fscanf(stdin, "%s", buf) ? : abort();

		char code = buf[0];

		/* Close window */
		if(code == 'X')
		{
			sleep(3);
			gtk_main_quit();
			break;
		}
		/* Snapshot */
		else if(code == 'S')
		{
			fscanf(stdin, "%s", buf) ? : abort();
			drawing_snapshot(cont, buf);
		}
		/* Shift */
		else if(code == 'L' || code == 'R' || code  == 'U' || code == 'D')
		{
			fscanf(stdin, "%hhu", &index) ? : abort();
			animate_shift(canvas, cont, index, code);
		}
		else
		{
			abort();
		}
	}

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
