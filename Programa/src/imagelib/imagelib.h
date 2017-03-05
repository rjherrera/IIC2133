#ifndef T0_LIB_IMAGE
#define T0_LIB_IMAGE

#include <stdint.h>

struct color
{
	/** Canal rojo */
	uint8_t R;
	/** Canal verde */
	uint8_t G;
	/** Canal azul */
	uint8_t B;
};
/** Representa un color en RGB */
typedef struct color Color;

struct image
{
	/** Matriz de colores que corresponde a la imagen */
	Color** pixels;
	/** Ancho de la imagen */
	uint16_t width;
	/** Alto de la imagen */
	uint16_t height;
};
/** Representa una imagen RGB como una matriz de colores */
typedef struct image Image;

/** Lee un archivo .png y entrega la imagen */
Image* img_png_read_from_file (char* filename);
/** Escribe el contenido de una imagen como archivo .png */
void   img_png_write_to_file  (Image* img, char* filename);

#endif /* end of include guard: T0_LIB_IMAGE */
