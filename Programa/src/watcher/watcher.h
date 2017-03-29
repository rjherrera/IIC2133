#ifndef T1_LIB_WATCHER
#define T1_LIB_WATCHER

/* ######################################################################## */
/* #                              WATCHER                                 # */
/* ######################################################################## */
/* #                                                                      # */
/* # Este módulo controla la ventana de interfaz gráfica "beholder" de    # */
/* # manera asíncrona desde tu programa. Eso quiere decir que puedes usar # */
/* # estas funciones sin miedo a que tu programa se cuelgue.              # */
/* #                                                                      # */
/* # El controlador es un singleton, así que puedes llamar las funciones  # */
/* # del watcher desde cualquier contexto en tu programa y funcionarán.   # */
/* ######################################################################## */

/** Abre una ventana de las dimensiones especificadas (en celdas) */
void watcher_open          (int height, int width);
/** Indica que la celda dada no contiene ningún tipo de construcción */
void watcher_set_grass     (int row, int col);
/** Dibuja una pieza que coincida con las delimitaciones especificadas.
    Sino, se pone en blanco */
void watcher_set_tile      (int row, int col, int u, int r, int d, int l);
/** Indica que la celda especificada está vacía */
void watcher_set_empty_cell(int row, int col);
/** Guarda una imagen .png con el estado actual de la ventana */
void watcher_snapshot      (char* filename);
/** Cierra la ventana */
void watcher_close         ();

#endif /* End of include guard: T1_LIB_WATCHER */
