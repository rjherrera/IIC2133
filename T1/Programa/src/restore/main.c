#include "../watcher/watcher.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define sleep_time 100000

struct piece {
    int id;
    int up;
    int right;
    int down;
    int left;
    int times;
    int rotations;
};

typedef struct piece Piece;

void symmetry(Piece piece){
	// poda de simetría, no rotar si no es necesario
    int u = piece.up;
    int r = piece.right;
    int d = piece.down;
    int l = piece.left;
    if ((u == d == l == r)) {
        piece.rotations = 0;
    }
    else if ((u == d) && (l == r)) {
        piece.rotations = 1;
    }
};

Piece new_piece(int id, int up, int right, int down, int left, int times){
    Piece pc;
    pc.id = id;
    pc.up = up;
    pc.right = right;
    pc.down = down;
    pc.left = left;
    pc.times = times;
    pc.rotations = 3;
    symmetry(pc);
    return pc;
};

int closed_frontiers(int*** grid, int height, int width, int row, int col){
    int cerradas = 0;
    if (row - 1 < 0) cerradas += 1 + grid[row + 1][col][0]; 			// abajo
    else if (height - row == 1) cerradas += 1 + grid[row - 1][col][0]; 	// arriba
    else cerradas += grid[row + 1][col][0] + grid[row - 1][col][0];		// abajo y arriba
    if (col - 1 < 0) cerradas += 1 + grid[row][col + 1][0]; 			// derecha
    else if (width - col == 1) cerradas += 1 + grid[row][col - 1][0];	// izquierda
    else cerradas += grid[row][col - 1][0] + grid[row][col + 1][0];		// derecha e izquierda
    return cerradas; // retorna la cantidad de fronteras cerradas
};

void rotate(Piece piece) {
	int u = piece.up; int r = piece.right; int d = piece.down; int l = piece.left;
	piece.up = l;
	piece.right = u;
	piece.down = r;
	piece.left = d;
}

bool is_solution(int*** grid, int height, int width){
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++){
			if (grid[i][j][0] == 0) return false;
		}
	}
	return true;
};

bool find_row_col(int*** grid, int height, int width, int* col, int* row){
	// busco el par i,j que tenga menos bordes libres o mas bordes cerrados (heuristica)
	for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if(!grid[i][j][0] && grid[i][j][2] >= 3){
                *row = i;
                *col = j;
                return true;
            }
        }
    }
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if(!grid[i][j][0] && grid[i][j][2] == 2){
                *row = i;
            	*col = j;
                return true;
            }
        }
    }
    return false;
}

bool place_candidate(int*** grid, Piece piece, int row, int col) {
	// poner la pieza en el lugar para así continuar
	return true;
}

void rollback() {
	// deshacer la postura de la pieza
}

bool backtracking(int*** grid, Piece* pieces, int k, int height, int width, int row, int col) {
	if (is_solution(grid, height, width)) return true;
	// heuristica de encontrar la row y col para probar
	find_row_col(grid, height, width, &row, &col);
	for (int i = 0; i < k; i++){
		Piece candidate = pieces[i];
		if (candidate.times) {
			for (int j = 0; j < candidate.rotations; j++){
				if (place_candidate(grid, candidate, row, col)){
					if (backtracking(grid, pieces, k, height, width, row, col)) return true;
					else rollback();
				}
				else rotate(candidate);
			}

		}
	}
	return false;
}

int main(int argc, char** argv) {
    if(argc != 3) {
        printf("Modo de uso: %s <mapa.txt> <solución.txt>\n", argv[0]);
        printf("\tmapa.txt: contiene la descripción del puzzle a resolver\n");
        printf("\tsolución.txt: ruta de archivo donde se guardará la solución\n");
        return 1;
    }

    // Lectura de archivo
    char* input_file = argv[1];
    FILE* file = fopen(input_file, "r");

    int rows;
    int cols;
    int grass;
    fscanf(file, "%d\n%d\n%d", &rows, &cols, &grass);

    // Creación de grid con piezas "empty"
    // grid[i][j] = [id, rot, bordes cerrados]
    int*** grid = (int***)malloc(sizeof(int**) * rows);
    for(int row = 0; row < rows; row++){
        grid[row] = (int**)malloc(sizeof(int*) * cols);
        for(int col = 0; col < cols; col++){
            grid[row][col] = (int*)calloc(3, sizeof(int));
            grid[row][col][0] = 0;
            grid[row][col][1] = 0;
        }
    }

    // Marcado de pastos
    for (int i = 0; i < grass; i++) {
        int rg, cg;
        fscanf(file, "%d %d", &rg, &cg);
        grid[rg][cg][0] = 1;
    }

    // marcado de fronteras
    for(int row = 0; row < rows; row++){
        for(int col = 0; col < cols; col++){
            grid[row][col][2] = closed_frontiers(grid, rows, cols, row, col);
        }
    }

    // creado de piezas posibles
    int k;
    fscanf(file, "%d", &k);
    Piece* piezas = calloc(k, sizeof(Piece));
    for (int i = 0; i < k; i++){
        int up, right, down, left, times;
        fscanf(file, "%d %d %d %d %d", &up, &right, &down, &left, &times);
        piezas[i] = new_piece(i + 1, up, right, down, left, times);
    }

    backtracking(grid, piezas, k, rows, cols, 0, 0);


    // int** pieces = calloc(k, sizeof(int*));
    // for (int i = 0; i < k; i++) {
    //     pieces[i] = calloc(5, sizeof(int*));
    //     for (int j = 0; j < 5; j++) fscanf(file, "%d", &pieces[i][j]);
    // }

    fclose(file);

    // Watcher
    watcher_open(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j][0] == 1) watcher_set_grass(i, j);
            usleep(sleep_time);
        }
    }



    /* Ejemplo de como usar la interfaz gráfica */
    /* Tambien incluye pausas por si quieres seguir tu programa paso a paso */

    // // Mapa de 3 filas y 4 columnas
    // watcher_open(3, 4);
    // // Inicialmente son todas pasto
    // for(int row = 0; row < 3; row++)
    // {
    //  for(int col = 0; col < 4; col++)
    //  {
    //      watcher_set_grass(row, col);
    //      // Hacemos una pausa entre comando y comando
    //      usleep(100000); // <- usleep(n) duerme el thread durante n microsegundos
    //  }
    // }

    // // Ponemos en la posicion (1,2) una celda G R C R
    // watcher_set_tile(1, 2, 1, 2, 3, 2);

    // // Generamos una imagen de la interfaz en ese momento dado
    // watcher_snapshot("foto.png");

    // // Ponemos en la posicion (2,3) una celda R C C R
    // watcher_set_tile(2, 3, 2, 3, 3, 2);

    // // Ponemos en la posicion (0,0) una pieza vacía
    // watcher_set_empty_cell(0, 0);

    // // Esperamos para alcanzar a ver que hay en la ventana
    // sleep(5); // <- sleep(n) duerme el thread durante n segundos

    /* TODO Para entregar tu tarea borra todos los sleeps y llamadas a watcher */

    // watcher_close();



    return 0;
}
