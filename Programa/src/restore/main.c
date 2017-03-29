#include "../watcher/watcher.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	if(argc != 3)
	{
		printf("Modo de uso: %s <mapa.txt> <solución.txt>\n", argv[0]);
		printf("\tmapa.txt: contiene la descripción del puzzle a resolver\n");
		printf("\tsolución.txt: ruta de archivo donde se guardará la solución\n");
		return 1;
	}

	/* Ejemplo de como usar la interfaz gráfica */
	/* Tambien incluye pausas por si quieres seguir tu programa paso a paso */

	// Mapa de 3 filas y 4 columnas
	watcher_open(3, 4);
	// Inicialmente son todas pasto
	for(int row = 0; row < 3; row++)
	{
		for(int col = 0; col < 4; col++)
		{
			watcher_set_grass(row, col);
			// Hacemos una pausa entre comando y comando
			usleep(100000); // <- usleep(n) duerme el thread durante n microsegundos
		}
	}

	// Ponemos en la posicion (1,2) una celda G R C R
	watcher_set_tile(1, 2, 1, 2, 3, 2);

	// Generamos una imagen de la interfaz en ese momento dado
	watcher_snapshot("foto.png");

	// Ponemos en la posicion (2,3) una celda R C C R
	watcher_set_tile(2, 3, 2, 3, 3, 2);

	// Ponemos en la posicion (0,0) una pieza vacía
	watcher_set_empty_cell(0, 0);

	// Esperamos para alcanzar a ver que hay en la ventana
	sleep(5); // <- sleep(n) duerme el thread durante n segundos

	/* TODO Para entregar tu tarea borra todos los sleeps y llamadas a watcher */

	watcher_close();

	return 0;
}
