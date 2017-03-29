#include "watcher.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WATCHER "./beholder"

static FILE* watcher = NULL;

/** Abre un watcher de las dimensiones especificadas */
void watcher_open(int height, int width)
{
  char command[256];
  sprintf(command, "%s %d %d", WATCHER, height, width);

  if(watcher) watcher_close();

  watcher = popen(command, "w");
}

/** Dibuja una pieza que coincida con las delimitaciones especificadas.
    Sino, se pone en blanco */
void watcher_set_tile(int row, int col, int u, int r, int d, int l)
{
	if(watcher)
	{
		if(fprintf(watcher, "TILE %d %d %d %d %d %d\n", row, col, u, r, d, l) < 0)
		{
			watcher_close();
		}
		else
		{
			fflush(watcher);
		}
	}
}

/** Indica que la celda dada no contiene ningún tipo de construcción */
void watcher_set_grass(int row, int col)
{
	if(watcher)
	{
		if(fprintf(watcher, "GRASS %d %d\n", row, col) < 0)
		{
			watcher_close();
		}
		else
		{
			fflush(watcher);
		}
	}
}

/** Indica que la celda especificada está vacía */
void watcher_set_empty_cell(int row, int col)
{
	if(watcher)
	{
		if(fprintf(watcher, "EMPTY %d %d\n", row, col) < 0)
		{
			watcher_close();
		}
		else
		{
			fflush(watcher);
		}
	}
}

/** Guarda una imagen .png con el estado actual del visualizador */
void watcher_snapshot(char* filename)
{
	if(watcher)
	{
		if(fprintf(watcher, "SNAPSHOT %s\n", filename) < 0)
		{
			watcher_close();
		}
		else
		{
			fflush(watcher);
		}
	}
}

/** Cierra el watcher */
void watcher_close()
{
  if(watcher)
  {
    if(fprintf(watcher, "%s\n", "END") >= 0)
    {
      fflush(watcher);
      pclose(watcher);
    }
    watcher = NULL;
  }
}
