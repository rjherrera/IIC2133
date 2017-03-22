#include <stdio.h>
#include <stdlib.h>
#include "../imagelib/imagelib.h"

/** Libera los recursos asociados a una imagen */
void img_destroy(Image* img)
{
	/* TODO: Liberar la memoria */
	int height = img -> height;
	for (int row = 0; row < height; row++) {
		free(img -> pixels[row]);
	}
	free(img -> pixels);
	free(img);
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

	/* TODO: Leer el kernel a partir del archivo */

	char* kernel_file = argv[2];

	FILE* file = fopen(kernel_file, "r");
	float data;
	int kernel_rows;
	int kernel_cols;
	fscanf(file, "%d", &kernel_rows);
    fscanf(file, "%d", &kernel_cols);

    float** kernel = malloc(kernel_rows * sizeof(float*));
    // float kernel[kernel_rows][kernel_cols]; sin mallocs
    for (int i = 0; i < kernel_rows; i++) {
        kernel[i] = malloc(kernel_cols * sizeof(float));
        for (int j = 0; j < kernel_cols; j++) {
            fscanf(file, "%f", &data);
            kernel[i][j] = data;
        }
    }

	fclose(file);

	/* TODO: Procesar la imagen */

	Image* new_img = img_png_read_from_file(input_file);
	int height = img -> height;
	int width = img -> width;
	int margin_rows = kernel_rows / 2;
	int margin_cols = kernel_cols / 2;
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			float suma[3] = {0};
			for (int i = row - margin_rows; i < row + margin_rows + 1; i++){
				for (int j = col - margin_cols; j < col + margin_cols + 1; j++){
					int img_i = i;
					int img_j = j;
					int kernel_i = i - row + margin_rows;
                    int kernel_j = j - col + margin_cols;
					if (i < 0) 			img_i = 0;
					if (j < 0) 			img_j = 0;
					if (i >= height) 	img_i = height - 1;
					if (j >= width)  	img_j = width - 1;
					suma[0] += img -> pixels[img_i][img_j].R * kernel[kernel_i][kernel_j];
					suma[1] += img -> pixels[img_i][img_j].G * kernel[kernel_i][kernel_j];
					suma[2] += img -> pixels[img_i][img_j].B * kernel[kernel_i][kernel_j];
				}
			}
			new_img -> pixels[row][col].R = suma[0];
			new_img -> pixels[row][col].G = suma[1];
			new_img -> pixels[row][col].B = suma[2];
		}
	}

	/* Guardamos la imagen como un archivo png */
	char* output_file = argv[3];
	img_png_write_to_file (new_img, output_file);

	/* Liberamos los recursos asociados a la imagen */
	img_destroy(img);
	img_destroy(new_img);

	return 0;
}
