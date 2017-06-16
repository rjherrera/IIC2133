#include "dictionary.h"
#include <stdlib.h>
#include "../../../random/pcg_basic.h"
#include <math.h>

#define TYPE "INCREMENTAL"
#define BUCKETS 512
#define RATIO 0.6667
#define RATE ""


uint32_t*** numbers;
int FIB_PREVIOUS = BUCKETS - 1;

uint32_t naive_hash(Puzzle* puzzle){
    uint32_t hash = 0;
    for (int i = 0; i < puzzle -> height; ++i) {
        for (int j = 0; j < puzzle -> width; ++j) {
            uint8_t color = puzzle -> matrix[i][j];
            hash ^= numbers[i][j][color];
        }
    }
    return hash;
}


void perfect_hash(mpz_t hash, Puzzle* puzzle){
	mpz_init(hash);
	for (int i = 0; i < puzzle -> height; ++i) {
        for (int j = 0; j < puzzle -> width; ++j) {
            uint8_t color = puzzle -> matrix[i][j];
            mpz_mul_2exp(hash, hash, 3);
            mpz_add_ui(hash, hash, color);
        }
    }
}


int incremental_hash(Puzzle* puzzle, Cell* prev, Operation operation){
    int hash = prev -> naive_hash;
    char direction = operation_direction(operation);
    uint8_t index = operation_index(operation);
    // undo prev changes (xor with prevs) and make new changes (xor with actuals)
    if (direction == 'R' || direction == 'L'){ // se esta moviendo la fila hacia algun lado
        for (int i = 0; i < puzzle -> width; ++i){ // fila fija, itero sobre columnas
            uint8_t previous_color = prev -> puzzle -> matrix[index][i];
            uint32_t previous_color_number = numbers[index][i][previous_color];
            uint8_t new_color = puzzle -> matrix[index][i];
            uint32_t new_color_number = numbers[index][i][new_color];
            hash ^= previous_color_number;
            hash ^= new_color_number;
            // printf("  Undo: M(%d,%d): %u -> %u\n", index, i, previous_color, previous_color_number);
            // printf("  Redo: M(%d,%d): %u -> %u\n", index, i, new_color, new_color_number);
        }
    }
    else { // se esta moviendo la columna hacia alguna altura
        for (int i = 0; i < puzzle -> height; ++i){ // columna fija, itero sobre filas
            uint8_t previous_color = prev -> puzzle -> matrix[i][index];
            uint32_t previous_color_number = numbers[i][index][previous_color];
            uint8_t new_color = puzzle -> matrix[i][index];
            uint32_t new_color_number = numbers[i][index][new_color];
            hash ^= previous_color_number;
            hash ^= new_color_number;
        }
    }
    return hash;
}

Cell* cell_init(Puzzle* puzzle){
    Cell* cell = malloc(sizeof(Cell));
    cell -> new_cell = true;
    cell -> puzzle = puzzle_clone(puzzle);
    cell -> next_cell = NULL;
    return cell;
}

/** Inicializa el diccionario y todo lo asociado */
Dictionary* dictionary_init(Puzzle* initial){
	// TODO Inicializa el diccionario y todo lo que sea necesario guardar en el
	if (TYPE == "INCREMENTAL"){
	    uint64_t seed = 0;
	    uint64_t seq = 0;
	    pcg32_srandom(seed, seq);
	    numbers = malloc(initial -> height * sizeof(uint32_t**));
	    for (int i = 0; i < initial -> height; ++i){
	        numbers[i] = malloc(initial -> width * sizeof(uint32_t*));
	        for (int j = 0; j < initial -> width; ++j){
	            numbers[i][j] = malloc(8 * sizeof(uint32_t));
	            for (int k = 0; k < 8; ++k){
	                numbers[i][j][k] = pcg32_random();
	            }
	        }
	    }
	}
    Dictionary* dict = malloc(sizeof(Dictionary));
    dict -> initial = initial;
    dict -> m = BUCKETS;
    dict -> used = 0;
    dict -> cells_array = malloc(dict -> m * sizeof(Cell*));
    for (int i = 0; i < dict -> m; ++i) dict -> cells_array[i] = NULL; // inicializar para evitar error de valgrind
    return dict;
}

/* Buscar o poner celda con encadenamiento (argumento hash es un uint32_t) */
Cell* find_or_place_cell(Dictionary* dict, Puzzle* state, uint32_t hash, int index){
    if (dict -> cells_array[index] == NULL){ // si no hay nadie en el primer elemento del bucket, se crea y se pone ahí
        Cell* new_cell = cell_init(state);
        new_cell -> naive_hash = hash;
        dict -> cells_array[index] = new_cell;
        dict -> used++;
        // printf("SE HIZO NEW CELL EN PRIMER ELEMENTO DEL BUCKET -> used:%d, hash:%u\n", dict -> used, hash);
        return new_cell;
    }
    Cell* actual_cell = dict -> cells_array[index]; // sino, se toma esa celda
    while (actual_cell != NULL){
        if (puzzle_equals(actual_cell -> puzzle, state)){
            // printf("ME ENCONTRO! hash:%u==%u\n", hash, actual_cell -> naive_hash);
            return actual_cell; // si es igual la retorno
        }
        else if (actual_cell -> next_cell == NULL){ // si no hay next, creo y la pongo
            // printf("NO HAY NEXT, ME PONGO CM NEW CELL EN EL NEXT\n");
            Cell* new_cell = cell_init(state);
            new_cell -> naive_hash = hash;
            actual_cell -> next_cell = new_cell;
            dict -> used++;
            return new_cell;
        }
        actual_cell = actual_cell -> next_cell; // si hay next avanzo
    }
    return NULL;
}


/* Buscar o poner celda con encadenamiento (argumento hash es un mpz_t) */
Cell* find_or_place_cell_perfect(Dictionary* dict, Puzzle* state, mpz_t hash, int index){
	if (dict -> cells_array[index] == NULL){ // si no hay nadie en el primer elemento del bucket, se crea y se pone ahí
        Cell* new_cell = cell_init(state);
        mpz_init_set(new_cell -> perfect_hash, hash);
        mpz_clear(hash);
        dict -> cells_array[index] = new_cell;
        dict -> used++;
        return new_cell;
    }
    Cell* actual_cell = dict -> cells_array[index]; // sino, se toma esa celda
    while (actual_cell != NULL){
        if (puzzle_equals(actual_cell -> puzzle, state)){
        	mpz_clear(hash);
            return actual_cell; // si es igual la retorno
        }
        else if (actual_cell -> next_cell == NULL){ // si no hay next, creo y la pongo
            Cell* new_cell = cell_init(state);
            mpz_init_set(new_cell -> perfect_hash, hash);
            mpz_clear(hash);
            actual_cell -> next_cell = new_cell;
            dict -> used++;
            return new_cell;
        }
		actual_cell = actual_cell -> next_cell; // si hay next avanzo
    }
    return NULL;
}


void place_cell(Dictionary* dict, Cell* cell){
	int index = 0;
	if (TYPE == "INCREMENTAL") index = cell -> naive_hash % dict -> m;
	else if (TYPE == "PERFECT") index = mpz_fdiv_ui(cell -> perfect_hash, dict -> m);
    if (!dict -> cells_array[index]){ // si no hay nadie en el primer elemento del bucket se pone ahí
        dict -> cells_array[index] = cell;
        dict -> used++;
        return;
    }
    Cell* actual_cell = dict -> cells_array[index]; // sino, se toma esa celda
    while (actual_cell != NULL){
        if (actual_cell -> next_cell == NULL){ // si no hay next, la pongo como next
            actual_cell -> next_cell = cell;
            dict -> used++;
            return;
        }
        else {
            actual_cell = actual_cell -> next_cell; // si hay next avanzo al next
        }
    }
}


bool is_prime(int number){
    // pseudo es_primo para numeros grandes
    int limit = (int) sqrt((float) number) + 1;
    for (int i = 2; i < limit; ++i)
        if (!(number % i)) return false;
    return true;
}


int get_new_table_size(int m){
    int new_m;
    if (RATE == "DOUBLE")
        new_m = m * 2;
    else if (RATE == "FIBONACCI"){
        new_m = m + FIB_PREVIOUS;
        FIB_PREVIOUS = m;
    }
    else if (RATE == "DOUBLE_PRIME"){
        new_m = m * 2 + 1;
        while (!is_prime(new_m)) new_m++;
    }
    else if (RATE == "TRIPLE")
        new_m = m * 3;
    else if (RATE == "RANDOM")
        new_m = m * (2 + pcg32_boundedrand(4));
    else
        new_m = m * 2 + 1;
    return new_m;
}


void rehash(Dictionary* dict){
    int previous_m = dict -> m;
    Cell** temp_array = malloc(previous_m * sizeof(Cell*));
    for (int i = 0; i < previous_m; ++i) temp_array[i] = dict -> cells_array[i]; // copiar el arreglo de buckets a temporal
    // FORMA DE INCREMENTAR EL M
    dict -> m = get_new_table_size(previous_m);
    dict -> used = 0;
    free(dict -> cells_array);
    dict -> cells_array = malloc(dict -> m * sizeof(Cell*));
    for (int i = 0; i < dict -> m; ++i) dict -> cells_array[i] = NULL; // inicializar para evitar error de valgrind
    for (int i = 0; i < previous_m; ++i){
        Cell* cell = temp_array[i];
        while (cell != NULL){
            Cell* actual_cell = cell;
            Cell* next_cell = cell -> next_cell;
            actual_cell -> next_cell = NULL;
            place_cell(dict, actual_cell);
            cell = next_cell;
        }
    }
    free(temp_array);
}


/** Obtiene la celda en la que se guarda ese estado durante todo el programa */
Cell* dictionary_get_cell(Dictionary* dict, Puzzle* state, Cell* prev, Operation op){
    // TODO esta es la función más importante que debes implementar

    int m = dict -> m;
    if (dict -> used / (float) m > RATIO){
        printf("\nREHASH(start) - Used: %d, Buckets: %d, Ratio: %f > %f\n", dict -> used, m, dict -> used / (float) m, RATIO);
        rehash(dict);
        printf("REHASH(end) - Used: %d, Buckets: %d, Ratio: %f < %f\n", dict -> used, dict -> m, dict -> used / (float) dict -> m, RATIO);
    }

    if (TYPE == "INCREMENTAL"){
        uint32_t hashed_puzzle;
        int index;
        if (prev){
            hashed_puzzle = incremental_hash(state, prev, op);
            index = hashed_puzzle % m;
            return find_or_place_cell(dict, state, hashed_puzzle, index);
        }
        else {
            hashed_puzzle = naive_hash(state);
            index = hashed_puzzle % m;
            return find_or_place_cell(dict, state, hashed_puzzle, index);
        }
    }
    else if (TYPE == "PERFECT"){
    	mpz_t hash_mpz;
		perfect_hash(hash_mpz, state);
		int index = mpz_fdiv_ui(hash_mpz, m);
		return find_or_place_cell_perfect(dict, state, hash_mpz, index);
    }

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
    for (int i = 0; i < dict -> m; ++i)
    {
    	Cell* cell = dict -> cells_array[i];
		while (cell != NULL){
			Cell* prev = cell;
			cell = cell -> next_cell;
			puzzle_destroy(prev -> puzzle);
			if (TYPE == "PERFECT") mpz_clear(prev -> perfect_hash);
			free(prev);
		}
    }
    free(dict -> cells_array);
    if (TYPE == "INCREMENTAL"){
	    int h = dict -> initial -> height;
	    int w = dict -> initial -> width;
	    for (int i = 0; i < h; ++i){
			for (int j = 0; j < w; ++j)
				free(numbers[i][j]);
			free(numbers[i]);
	    }
	    free(numbers);
    }
    free(dict);
}
