#include <stdio.h>
#include <stdlib.h>
#include "../imagelib/imagelib.h"

/** Libera los recursos asociados a una imagen */
void img_destroy(Image* img)
{
	/* TODO Liberar la memoria */
}

int main(int argc, char *argv[])
{
	/* El programa recibe 3 parametros */
	if(argc != 4)
	{
		printf("Modo de uso: %s <input.png> <kernel.txt> <output.png>\n", argv[0]);
		printf("\t<input.png> es la imagen a filtrar\n");
		printf("\t<kernel.txt> es el filtro a usar\n");
		printf("\t<output.png> es donde se guardará la imagen resultante\n");
		return 1;
	}

	/* Leemos la imagen a partir del archivo */
	char* input_file = argv[1];
	Image* img = img_png_read_from_file(input_file);

	/** TODO: Leer el kernel a partir del archivo */
	char* kernel_file = argv[2];

	/* TODO: Procesar la imagen */
	/*
	* A modo de ejemplo, este código invierte los colores de la imagen
	* Puedes crear una imagen trabajar en ella y no modificar la original
	*/
	for(int row = 0; row < img -> height; row++)
	{
		for(int col = 0; col < img -> width; col++)
		{
			img -> pixels[row][col].R = 255 - img -> pixels[row][col].R;
			img -> pixels[row][col].G = 255 - img -> pixels[row][col].G;
			img -> pixels[row][col].B = 255 - img -> pixels[row][col].B;
		}
	}

	/* Guardamos la imagen como un archivo png */
	char* output_file = argv[3];
	img_png_write_to_file (img, output_file);

	/* Liberamos los recursos asociados a la imagen */
	img_destroy(img);

	return 0;
}
