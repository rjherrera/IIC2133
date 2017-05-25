#!/bin/sh

#Make the whole pipe error if a command fails (timeout |  judge)
set -o pipefail

TIMEFORMAT=%R
# Para ahorranos el memory dump de los crashes
export MALLOC_CHECK_=2

ENTROPY=$1
HEIGHT=$2
WIDTH=$3
TIMES=$4
COLUMNS=$5
