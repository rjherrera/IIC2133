#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../music/melody.h"
#include "suffix.h"
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

	// preprocesar la melodía
	// char *string = string_from_melody(melody);

	// int notes_number = melody -> length;
	int notes_number = 7;
	char* palabra[notes_number];
	// banana$ = "(1|1)(1|2)(1|3)(1|2)(1|3)(1|2)" <- b=1|1, a=1|2, n=1|3
	//char *prepa[7] = {"(1|1)", "(1|2)", "(1|3)", "(1|2)", "(1|3)", "(1|2)", "$"};
	char* prepa[7] = {"b", "a", "n", "a", "n", "a", "$"};
	for (int i = 0; i < notes_number; ++i)
	{
		palabra[i] = malloc(strlen("a") + 1);
		strcpy(palabra[i], prepa[i]);
	}

	// imprimir string inicial
	for (int i = 0; i < notes_number; ++i) printf("%s", palabra[i]);
	printf("\n");

	// armar el trie a partir del string
	TrieNode* root = build_trie(notes_number, palabra);

	// imprimir el trie con indicador de profundidad
    // print_trie(root, 0);
    printf("Suffixes: %d\n", count_leafs(root));
    // PROBAR SI UN SUBSTRING ESTÁ EN LA PALABRA

    // substring como lista de caracteres (o strings)
    char* substring[2] = {"a","n"};
    int subs_len = 2;
    // // imprimir el substring en cuestion
    for (int i = 0; i < subs_len; ++i) printf("%s", substring[i]);
    printf(":\n");
    int appearences = 0;
    printf(" Exists: %d\n", substring_exists(root, substring, subs_len, &appearences));
    printf(" Count: %d\n", appearences);


    return 0;


	printf("Largo melodía: %zu\n", melody -> length);
	for (int i = 0; i < melody -> length; ++i)
	{
		printf("L:%u | N:%hhu\n", melody -> element_array[i].length, melody -> element_array[i].note);
	}

	int suffix_array[melody -> length];

	for (int i = 0; i < melody -> length; ++i)
	{
		suffix_array[i] = melody -> element_array[i].note;
	}

	/* Obtiene el midi a partir de la melodía resultante */
	midi = melody_to_midi(melody);

	/* Guarda el midi como archivo */
	midi_to_file(midi, argv[3]);

	/* Destruye el midi */
	midi_destroy(midi);

	/* Destruye la melodía */
 	melody_destroy(melody);

	return 0;
}
