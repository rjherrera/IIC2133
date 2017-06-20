#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

#define CELL_SIZE 128.0
#define MAX_SIZE 800.0

/** Escala */
double scale = 1.0;
/** Thread encargado de leer el input */
pthread_t* update_thread;
/** El alto de la matriz */
int height;
/** El ancho de la matriz */
int width;
/** Tablero indicando la imagen a la que corresponde cada celda */
cairo_surface_t*** board;
/** Imagen de una celda que no tiene celda */
cairo_surface_t* tile_not;
/** Imagen de una celda sin construcciones */
cairo_surface_t* tile_grass;
/** Matriz de 4 dimensiones que contiene las imagenes de las celdas */
cairo_surface_t***** tile_sprites;

/** Entrega un indice para saber a donde buscar la imagen en disco */
static char get_index_char(int index)
{
	switch(index)
	{
		case 1: return 'g';
		case 2: return 'r';
		case 3: return 'c';
		default: abort();
	}
}

/** Busca la pieza que corresponde.Si no la encuentra, la deja en blanco */
cairo_surface_t* get_tile(int row, int col, int u, int r, int d, int l)
{
	/* Obtenemos el tile correspondiente */
	cairo_surface_t* tile = tile_sprites[u][r][d][l];

	/* Si el tile no existe, entonces dibujamos una celda en blanco */
	if(!tile)
	{
		printf("ok\n");
		tile = tile_not;
	}

	/* Entregamos la celda que corresponde */
	return tile;
}

/** Dibuja la imagen en la ventana */
gboolean draw(GtkWidget* widget, cairo_t* cr, gpointer data)
{
	cairo_scale(cr, scale, scale);
  /* Color de fondo */
	cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_paint(cr);

  for(int row = 0; row < height; row++)
  {
    for(int col = 0; col < width; col++)
    {
			cairo_set_source_surface(cr, board[row][col], col * CELL_SIZE, row * CELL_SIZE);
			cairo_paint(cr);
    }
  }

  return TRUE;
}

/* Guarda una imagen .png con el estado actual de la ventana */
void snapshot(char* filename)
{
	double window_width = CELL_SIZE * width;
	double window_height = CELL_SIZE * height;

	/* Imprimimos las imagenes del tablero */
	cairo_surface_t* surface;
	cairo_t *cr;

	surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, window_width, window_height);
	cr = cairo_create(surface);

	/* Reseteamos el zoom para generar la imagen */
	double aux = scale;
	scale = 1.0;

	/* Dibuja el estado actual */
	draw(NULL, cr, NULL);

	/* Y volvemos a la que le corresponde */
	scale = aux;

	cairo_surface_write_to_png (surface, filename);

	cairo_surface_destroy(surface);
	cairo_destroy(cr);
}

/** Funcion que lee la siguiente jugada y actualiza el tablero segun eso */
void* update(void* canvas)
{
  int row, col;
	int l, r, d, u;
  char command[16];

  while(update_thread)
  {
    /* Si alguno de los numeros falla, dejamos de llamar */
    if(fscanf(stdin, "%s", command))
    {
      if(!strcmp(command, "END"))
      {
				gtk_main_quit();
				break;
      }
			else if(!strcmp(command, "TILE"))
			{
				if(!scanf("%d %d %d %d %d %d", &row, &col, &u, &r, &d, &l)) break;
				board[row][col] = get_tile(row, col, u, r , d, l);
			}
			else if(!strcmp(command, "GRASS"))
			{
				if(!scanf("%d %d", &row, &col)) break;
				board[row][col] = tile_grass;
			}
			else if(!strcmp(command, "EMPTY"))
			{
				if(!scanf("%d %d", &row, &col)) break;
				board[row][col] = tile_not;
			}
			else if(!strcmp(command, "SNAPSHOT"))
			{
				char filename[64];
				if(!scanf("%s",filename)) break;
				snapshot(filename);
			}
			else
			{
				break;
			}
    }
    else
    {
      break;
    }
		/* Actualizamos la ventana de manera asíncrona */
    gtk_widget_queue_draw(canvas);
  }

  pthread_exit(NULL);
}

/** Inicializa el thread que animará el programa */
void spawn_updater(GtkWidget *widget, gpointer user_data)
{
  /* Inicializamos el thread */
  update_thread = malloc(sizeof(pthread_t));
  /* Y lo lanzamos */
  pthread_create(update_thread, NULL, update, widget);
}

double DegreesToRadians( double degrees )
{
    return((double)((double)degrees * ( (double)M_PI/(double)180.0 )));
}

cairo_surface_t* cairo_img_rotate(cairo_surface_t* image, double deg)
{
	int w = cairo_image_surface_get_width(image);
	int h = cairo_image_surface_get_height(image);

	cairo_surface_t* target = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);

	cairo_t* cr = cairo_create(target);

	cairo_translate(cr, w*0.5, h*0.5);
  cairo_rotate(cr, DegreesToRadians( deg ));
  cairo_translate(cr, - w*0.5, -h*0.5);

	cairo_set_source_surface(cr, image, 0, 0);
	cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
	cairo_paint(cr);

	cairo_destroy(cr);

	return target;
}

bool file_exists(char* filename)
{
	if(access(filename, F_OK ) != -1)
	{
		return true;
	}
	else
	{
	  return false;
	}
}

/* Inicializa todas las imágenes del problema */
void tile_sprites_init()
{
	tile_not = cairo_image_surface_create_from_png("assets/oooo.png");
	tile_grass = cairo_image_surface_create_from_png("assets/xxxx.png");


	char filename[16] = "assets/????.png";


	bool**** images = calloc(4, sizeof(bool***));


	tile_sprites = calloc(4, sizeof(cairo_surface_t****));
	for(int u = 0; u < 4; u++)
	{
		tile_sprites[u] = calloc(4, sizeof(cairo_surface_t***));
		images[u] = calloc(4, sizeof(bool**));
		for(int r = 0; r < 4; r++)
		{
			tile_sprites[u][r] = calloc(4, sizeof(cairo_surface_t**));
			images[u][r] = calloc(4, sizeof(bool*));
			for(int d = 0; d < 4; d++)
			{
				images[u][r][d] = calloc(4, sizeof(bool));
				tile_sprites[u][r][d] = calloc(4, sizeof(cairo_surface_t*));
				for(int l = 0; l < 4; l++)
				{
					/* Si ya pasamos por aquí, se ignora */
					if(images[u][r][d][l]) continue;

					/* Si alguno es cero (edge no existe) se pone una pieza en blanco */
					if(!(u && r && d && l)) continue;

					filename[7]  = get_index_char(u);
					filename[8]  = get_index_char(r);
					filename[9]  = get_index_char(d);
					filename[10] = get_index_char(l);

					if(file_exists(filename))
					{
						tile_sprites[u][r][d][l] = cairo_image_surface_create_from_png(filename);
						images[u][r][d][l] = true;
					}
				}
			}
		}
	}

	for(int u = 0; u < 4; u++)
	{
		for(int r = 0; r < 4; r++)
		{
			for(int d = 0; d < 4; d++)
			{
				for(int l = 0; l < 4; l++)
				{
					if(images[u][r][d][l])
					{
						if(!images[l][u][r][d])
						{
							tile_sprites[l][u][r][d] = cairo_img_rotate(tile_sprites[u][r][d][l], 90);
							images[l][u][r][d] = true;
						}
						if(!images[d][l][u][r])
						{
							tile_sprites[d][l][u][r] = cairo_img_rotate(tile_sprites[l][u][r][d], 90);
							images[d][l][u][r] = true;
						}
						if(!images[r][d][l][u])
						{
							tile_sprites[r][d][l][u] = cairo_img_rotate(tile_sprites[d][l][u][r], 90);
							images[r][d][l][u] = true;
						}
					}
					else
					{
						tile_sprites[u][r][d][l] = tile_not;
					}
				}
			}
		}
	}

	for(int u = 0; u < 4; u++)
	{
		for(int r = 0; r < 4; r++)
		{
			for(int d = 0; d < 4; d++)
			{
				free(images[u][r][d]);
			}
			free(images[u][r]);
		}
		free(images[u]);
	}
	free(images);


	/* Inicialmente el tablero viene completamente vacío */
	board = calloc(height, sizeof(cairo_surface_t**));
	for(int row = 0; row < height; row++)
	{
		board[row] = calloc(width, sizeof(cairo_surface_t*));
		for(int col = 0; col < width; col++)
		{
			board[row][col] = tile_not;
		}
	}
}


/* Libera todos los recursos asociados */
void tile_sprites_destroy()
{
	for(int u = 0; u < 4; u++)
	{
		for(int r = 0; r < 4; r++)
		{
			for(int d = 0; d < 4; d++)
			{
				for(int l = 0; l < 4; l++)
				{
					if(tile_sprites[u][r][d][l] != tile_not)
					{
						cairo_surface_destroy(tile_sprites[u][r][d][l]);
					}
				}
				free(tile_sprites[u][r][d]);
			}
			free(tile_sprites[u][r]);
		}
		free(tile_sprites[u]);
	}
	free(tile_sprites);

	cairo_surface_destroy(tile_grass);
	cairo_surface_destroy(tile_not);

	for(int row = 0; row < height; row++)
	{
		free(board[row]);
	}
	free(board);

}

bool check_parameters(int argc, char** argv)
{
  if(argc != 3) return false;
  return true;
}

/** Visualiza la imagen construida por el renderer */
int main(int argc, char** argv)
{
  /* Revisamos que los parametros entregados sean correctos */
  if(!check_parameters(argc, argv)) return 1;

  /* Cargamos el puzzle */
	height = atoi(argv[1]);
	width = atoi(argv[2]);

	/* Inicializamos las imágenes */
	tile_sprites_init();

  /* Se cierra el canal para errores para que GTK no moleste */
  fclose(stderr);

  /* Inicializar GTK */
  gtk_init(0, NULL);

  /* Inicializar ventana */
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  /* Inicializar canvas */
  GtkWidget* canvas = gtk_drawing_area_new();

  /* Dimensiones del canvas */
  double window_width = CELL_SIZE * width * scale;
  double window_height = CELL_SIZE * height * scale;


	/* Ajustamos el tamaño de la ventana para que no supere los 800px */
	if(window_width > MAX_SIZE || window_height > MAX_SIZE)
	{
		if(window_width > window_height)
		{
			scale = MAX_SIZE / (CELL_SIZE * width);
		}
		else
		{
			scale = MAX_SIZE / (CELL_SIZE * height);
		}
		window_width = CELL_SIZE * width * scale;
		window_height = CELL_SIZE * height * scale;
	}


	gtk_widget_set_size_request(canvas, window_width, window_height);

  /* Ligar eventos */
  g_signal_connect(canvas, "draw", G_CALLBACK(draw), NULL);
  g_signal_connect(canvas, "realize", G_CALLBACK(spawn_updater), NULL);

  /* Meter canvas a la ventana */
  gtk_container_add(GTK_CONTAINER(window), canvas);

	/* Centramos la ventana y le ponemos nombre */
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);
	gtk_window_set_title(GTK_WINDOW(window), "watcher");

  /* Mostrar todo */
  gtk_widget_show(canvas);
  gtk_widget_show(window);

  /* Comenzamos la ejecucion de GTK */
  gtk_main();

	/* Liberamos la memoria */
  free(update_thread);
	update_thread = NULL;
	tile_sprites_destroy();

  return 0;
}
