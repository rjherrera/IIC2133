#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include "animation.h"
#include "../puzzle/puzzle.h"

/** El thread encargado de actualizar el contenido de la ventana */
static pthread_t* update_thread;

/** Lleva a cabo la actualización del tablero */
void* update(void* ptr)
{
	/* Desencapsula los parámetros */
	void** param = ptr;
	GtkWidget* canvas = param[0];
	Puzzle* puz = param[1];
	free(param);

	/* Row, Column & Value */
	uint8_t r, c, v;

	char buf[8];

	while(true)
	{
		fscanf(stdin, "%s", buf);

		if(!strcmp(buf, "SET"))
		{
			fscanf(stdin, "%hhu %hhu %hhu", &r, &c, &v);
			puz -> matrix[r][c] = v;
			gtk_widget_queue_draw(canvas);
		}
		else if(!strcmp(buf, "END"))
		{
			break;
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
