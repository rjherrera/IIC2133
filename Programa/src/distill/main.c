#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../music/melody.h"
#include "suffix.h"
#include "melody_helpers.h"
#include <stdbool.h>

/** Revisa que los parámetros sean válidos */
void check_parameters(int argc, char *argv[])
{
	/* Si se recibieron 4 parámetros */
	if(argc == 5)
	{
		/* Se revisa si es el flag del bonus */
		if(!strcmp(argv[4], "-b"))
		{
			/* Si es, entonces termina el programa sin errores */
			exit(0);
			// TODO si implementas el bonus, entonces tu programa tiene que seguir
		}
		/* Sino, es un parámetro inválido. */
		/* El if de abajo se encargará de terminar el programa */
	}

	/* El programa recibe 3 parametros. Si tenemos algo distinto entonces... */
	if(argc != 4)
	{
		printf("Modo de uso: %s <input.mid> <mu> <output.mid>\n", argv[0]);
		printf("\t<input.midi> es el archivo midi con el que trabajará \n");
		printf("\t<mu> es la cantidad mínima de notas de la melodía repetida \n");
		printf("\t<output.midi> es donde se guardará el audio \n");
		/* Fin del programa, codigo de error 1 */
		exit(1);
	}
}


// int is_in(TrieNode *node, TrieNode **node_array, int size){
// 	for (int i = 0; i < size; ++i)
// 	{
// 		printf(" kids:%s", node_array[i] -> value);
// 	}
// 	printf("\n");
//     for (int i = 0; i < size; i++) {
//     	printf(" Esta: %d |%s==%s\n", !strcmp(node_array[i] -> value, node -> value), node_array[i] -> value, node -> value);
//         if (!strcmp(node_array[i] -> value, node -> value)) return i + 1;
//     }
//     return 0;
// }


int main(int argc, char *argv[])
{
	/* Revisa que los parámetros sean correctos antes de empezar */
	check_parameters(argc, argv);

	/* Lee el midi */
	MIDI* midi = midi_from_file(argv[1]);

	/* Obtiene la melodía a partir del midi*/
	Melody* melody = melody_from_midi(midi);

	/* Destruye el midi, ya no se necesita*/
	midi_destroy(midi);


	// TODO Procesar la melodía
	int mu = strtol(argv[2], NULL, 10);
	// preprocesar la melodía
	int notes_number = melody -> length;
	printf("%d\n", notes_number);
	char* string[notes_number + 1];
	// char* orig_string[notes_number + 1];
	string_from_melody(&string, melody, notes_number); //, &orig_string);
	string[notes_number] = "$";
	// orig_string[notes_number] = "$";

	// PRUEBA BANANA
	// int notes_number = 8;
    // printf("%d\n", notes_number);
    // char* string[notes_number + 1];
    // char *prepa[9] = {"b", "t", "n", "a", "a", "n", "a", "a"};
    // for (int i = 0; i < notes_number; ++i){
    //  string[i] = malloc(2);
    //  strcpy(string[i], prepa[i]);
    // }
    // string[notes_number] = "$";
    // PRUEBA BANANA

	// banana$ = "(1|1)(1|2)(1|3)(1|2)(1|3)(1|2)" <- b=1|1, a=1|2, n=1|3
	// char *prepa[7] = {"(1|1)", "(1|2)", "(1|3)", "(1|2)", "(1|3)", "(1|2)", "$"};
	// char *prepa[7] = {"b", "a", "n", "a", "n", "a", "$"};
	// for (int i = 0; i < notes_number; ++i){
	// 	string[i] = malloc(strlen("a") + 1);
	// 	strcpy(string[i], prepa[i]); // prepa en vez de buff
	// }

	// imprimir string inicial
	for (int i = 0; i < notes_number + 1; ++i) printf("%s", string[i]);
	printf("\n");
	// for (int i = 0; i < notes_number + 1; ++i) printf("%s", orig_string[i]);
	// printf("\n");

	// armar el trie a partir del string
	TrieNode **candidates_array = malloc(10 * notes_number * sizeof(TrieNode*));
	int candidates_number = 0;
	TrieNode* root = build_trie(notes_number + 1, string, melody -> element_array, mu, candidates_array, &candidates_number);

	// imprimir el trie con indicador de profundidad
	level_trie(root, 0);
    // print_trie(root, 0);
    // printf("Suffixes: %d\n", count_leafs(root));


    // PROBAR SI UN SUBSTRING ESTÁ EN LA PALABRA
    // substring como lista de caracteres (o strings)
    // char* substring[3] = {"(-4|512|0)","(4|512|0)","(-4|512|0)"};
    // int subs_len = 3;
    // // imprimir el substring en cuestion
    // printf("'");
    // for (int i = 0; i < subs_len; ++i) printf("%s", substring[i]);
    // printf("':\n");
    // int appearences = 0;
    // printf(" Exists: %d\n", substring_exists(root, substring, subs_len, &appearences));
    // printf(" Count: %d\n", appearences);


    // VER MEJOR CANDIDATO
    TrieNode* actual_max = root;
    for (int i = 0; i < candidates_number; ++i) {
    	if (candidates_array[i] -> passes > actual_max -> passes){
    		actual_max = candidates_array[i];
    	}
    	else if (candidates_array[i] -> passes == actual_max -> passes){
    		if (candidates_array[i] -> level > actual_max -> level){
    			actual_max = candidates_array[i];
    		}
    	}
    	// printf("Candidato %d: %s\n", i, candidates_array[i] -> value);
    }
    printf("\nMejor: Node(%s)\n", actual_max -> value);
    printf("Subs + repetido de largo >= %d: ", mu);
    int soul_length = 1;
    print_route(actual_max, &soul_length);


	// EXPORTAR MELODIA
    TrieNode* current = actual_max;
    Element *soul_notes_array = malloc((soul_length) * sizeof(Element));
    int iter = 0;
    while (strcmp(current -> value, "ROOT")){
    	// printf("%s\n", current -> value);
    	soul_notes_array[iter] = current -> note_2;
    	if (!strcmp(current -> parent -> value, "ROOT")){
    		soul_notes_array[iter + 1] = current -> note_1;
    	}
        current = current -> parent;
        iter++;
    }

    // printf("len(soul): %d\n", soul_length);
	midi = melody_to_midi(melody);

	// printf("%d\n", soul_notes_array[soul_length-1].note);

	Melody* melody_output = melody_from_midi(midi);
	if (soul_length > 1){
		melody_output -> length = soul_length;
		// Element guide = melody_output -> element_array[0];
		melody_output -> element_array = malloc((soul_length) * sizeof(Element));
		for (int i = 0; i < soul_length; ++i){
			Element elemento = soul_notes_array[i];
			int* md = malloc(2 * sizeof(int));
			memcpy(md, elemento.metadata, 2 * sizeof(int));
			elemento.metadata = md;
			// printf("%d|%d|%d\n", elemento.note, elemento.length, elemento.type);
			melody_output -> element_array[soul_length - i - 1] = elemento;
		}
	}


	// printf("hola\n");
	// for (int i = 0; i < soul_length - 1; ++i){
	// 	Element h = melody_output -> element_array[i];
	// 	printf("%d;%d;%d\n", h.note, h.length, h.type);
	// }


	/* Obtiene el midi a partir de la melodía resultante */
	midi = melody_to_midi(melody_output);

	/* Guarda el midi como archivo */
	midi_to_file(midi, argv[3]);

	/* Destruye el midi */
	midi_destroy(midi);

	/* Destruye la melodía */
 	melody_destroy(melody);
 	melody_destroy(melody_output);

	return 0;
}
