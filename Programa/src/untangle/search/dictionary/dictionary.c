#include "dictionary.h"
#include <stdlib.h>

/** Inicializa el diccionario y todo lo asociado */
Dictionary* dictionary_init(Puzzle* initial)
{
	// TODO Inicializa el diccionario y todo lo que sea necesario guardar en el

	return NULL;
}
/** Obtiene la celda en la que se guarda ese estado durante todo el programa */
Cell* dictionary_get_cell(Dictionary* dict, Puzzle* state, Cell* prev, Operation op)
{
	// TODO esta es la función más importante que debes implementar

	// Existen dos casos: El estado "state" ya está en la tabla o no.
	// Si ya está, se retorna la celda en la que está guardado
	// Si no está, se crea una nueva celda, se guarda en la tabla y se retorna

	// Asegurate de leer src/puzzle/puzzle.h para ver la definición del struct
	// En la parte de abajo están las funciones que podrían interesarte

	// TODO Para cálculo incremental:
	// "prev" es la celda en la que estaba guardado el estado anterior.
	// Si "prev" es NULL es porque no existe "estado anterior"
	// En ese caso hay que calcular el hash completo.
	// "op" es la operación que generó "state" a partir del estado anterior

	// Asegurate de leer src/puzzle/operation.h para ver como interpretarla
	// En la parte de abajo están las funciones que podrían interesarte


	// En caso de que quieras usar números aleatorios, en la carpeta src/random
	// hay una librería que te permite generarlos de manera estandarizada

	return NULL;
}

/** Libera los recursos asociados a la tabla de hash */
void dictionary_free(Dictionary* dict)
{
	//TODO libera todos tus recursos
}
