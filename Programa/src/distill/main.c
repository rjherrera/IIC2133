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

	// creacion de un "string" de formato ()
	//int notes_number = melody -> element_array;
	int notes_number = strlen(argv[3]);
	char* palabra = argv[3];

	TrieNode* root = create_node("ROOT", notes_number);
	TrieNode* current = root;
    for (int i = 0; i < notes_number; i++) {
    	current = root;
    	printf("%s\n", current -> value);
        for (int j = i; j < notes_number; j++){
        	printf("Current: %s\n", current -> value);
        	char* value = palabra[j];
        	TrieNode* potential_node = create_node(&value, notes_number);
        	int k = current -> n_kids;
        	int present = is_in(potential_node, current -> kids, current -> n_kids);
        	if (!present){
        		current -> kids[k] = potential_node;
        		current -> n_kids = current -> n_kids + 1;
        		printf("  Hijos de %s: %d\n", current->value, current -> n_kids);
        		printf("  Nuevo in %s: Node(%c)\n", current->value, value);
        	}
        	else {
        		// printf("  Ya estaba: %c|%d de %d| en %s\n", value, present, current -> n_kids, current -> value);
                printf("  Ya estaba: %c en Node(%s)[%d]\n", value, current -> value, present - 1);

        		k = present - 1;
        	}
        	current = current -> kids[k];
        }
    }
    // for (int i = 0; i < root -> n_kids; ++i)
    // {
    // 	printf("jjj%s\n", root -> kids[i] -> value);
    // }

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
