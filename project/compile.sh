#!/bin/bash
# change GEOM_CASE based on geometry case:
#  0: 100 rabbits, 2 foxes, 1.0 vegetation in all tiles
#  1: 400 rabbits in one corner, 20 foxes in another, 10 rabbits elsewhere. 0.3 vegetation
#  2: 2 rabbits in all tiles, 0 foxes, vegetation is 0.5
mpicc -O3 -DGEOM_CASE=1 -lm main.c -o main