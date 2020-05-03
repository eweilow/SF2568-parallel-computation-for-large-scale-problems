#!/bin/bash
set -e

export OMPI_MCA_btl_vader_single_copy_mechanism=none

mpicc -O3 -DGEOM_W=8 -DGEOM_H=8 -DGEOM_CASE=1 -DSAVE_DATA=1 -lm main.c -o main
mpirun -np 4 main 2 2