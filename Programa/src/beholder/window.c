#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>
#include "window.h"
#include "drawing.h"
#include "animation.h"

/** Dibuja la imagen en la ventana */
gboolean draw(GtkWidget* widget, cairo_t* cr, gpointer data)
{
	return canvas_draw(cr, data);
}

/** Inicializa el canvas de acuerdo al tablero que se va a dibujar */
double gtk_canvas_init(GtkWidget* canvas, Puzzle* puz)
{
	/* Tamaño por defecto del canvas */
	double cell_size     = CELL_SIZE;
	double canvas_height = cell_size * (puz -> height + 1);
	double canvas_width  = cell_size * (puz -> width + 1);

	/* Restricción de tamaño */
	double scale = MAX_DIMENSION / fmax(canvas_width, canvas_height);

	/* Escalamiento */
	cell_size     *= scale;
	canvas_height *= scale;
	canvas_width  *= scale;

	gtk_widget_set_size_request(canvas, canvas_width, canvas_height);

	return cell_size;
}

void window_create(Content* cont)
{
	/* Se cierra el canal para errores para que GTK no moleste */
  fclose(stderr);

  /* Inicializar GTK */
  gtk_init(0, NULL);

	/* Inicializar ventana */
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(animation_abort), NULL);

  /* Inicializar canvas */
  GtkWidget* canvas = gtk_drawing_area_new();
	cont -> cell_size = gtk_canvas_init(canvas, cont -> puz);

	/* Ligar eventos */
  g_signal_connect(canvas, "draw", G_CALLBACK(draw), cont);
  g_signal_connect(canvas, "realize", G_CALLBACK(animation_init), cont -> puz);

	/* Meter canvas a la ventana */
  gtk_container_add(GTK_CONTAINER(window), canvas);

  /* Mostrar todo */
  gtk_widget_show(canvas);
  gtk_widget_show(window);

	/* Comenzamos la ejecución de GTK */
  gtk_main();
}
