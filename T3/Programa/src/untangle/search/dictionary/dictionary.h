#pragma once
#include <gmp.h>
#include "../../../puzzle/operation.h"
#include "../info.h"

/****************************************************************************/
/*                               Diccionario                                */
/****************************************************************************/
/* Esta estructura de datos asocia un estado de la búsqueda con su          */
/* información. Dado que esto depende de las propiedades de la búsqueda,    */
/* esta es manejada 100% por el código de la búsqueda. Para esto, tu tabla  */
/* DEBE CUMPLIR con que una vez que se inserta un elemento, la dirección en */
/* memoria del "struct hash_cell" en el que queda es LA MISMA a lo largo de */
/* todo el programa, para permitir a la búsqueda modificar los valores de   */
/* esta información cuando estime pertinente. Cuando crees una "hash_cell"  */
/* debes indicar en "new_cell" que ha sido recién creada.                   */
/****************************************************************************/

/** Contiene la asociación <estado,info> de la tabla de hash */
struct hash_cell
{
	//TODO Agrega todo lo quer estimes necesario guardar en una celda aca
    uint32_t naive_hash;
    mpz_t perfect_hash;
    Puzzle* puzzle;
    struct hash_cell* next_cell;
	/** Indica que esta celda ha sido recién creada */
	bool new_cell;
	/** La información que caracteriza a esta versión del estado */
	Info info;
};

typedef struct hash_cell Cell;

/** Asocia un estado conocido con su información mediante una tabla de hash */
struct hash_table
{
	//TODO completar esta struct para que funcione como diccionario
    int m;
    int used;
    Puzzle* initial;
    Cell** cells_array;
};

/** Asocia un estado conocido con su información mediante una tabla de hash */
typedef struct hash_table Dictionary;

/** Inicializa el diccionario y todo lo asociado */
Dictionary* dictionary_init     (Puzzle* initial);
/** Obtiene la celda en la que se guarda ese estado durante todo el programa */
Cell*       dictionary_get_cell (Dictionary* dict, Puzzle* state, Cell* prev, Operation op);
/** Libera los recursos asociados a la tabla de hash */
void        dictionary_free     (Dictionary* dict);
