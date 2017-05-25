#!/bin/sh

TIMEFORMAT=%R
# Para ahorranos el memory dump de los crashes
export MALLOC_CHECK_=2

# Parámetros del programa
ENTROPY=$1
HEIGHT=$2
WIDTH=$3
TIMES=$4
COLUMNS=$5

# Nombre del archivo de output
FILENAME="table.csv"
# Elimina el archivo de output
rm -f $FILENAME
# Crea el archivo de output
touch $FILENAME
# Imprime los nombres de las columnas al archivo
echo $COLUMNS > $FILENAME

# Corre el programa TIMES veces, usando cada vez una semilla distinta
for i in $(seq 1 $TIMES); do
	# Imprime el output del programa al final del archivo
	./generator $i $ENTROPY $HEIGHT $WIDTH | ./untangle -s >> $FILENAME
done
