#!/bin/bash
#./generateTestData.py $1
mpirun -np 8 ./matrixmultiply data_$1_a.matrix data_$1_b.matrix data_$1_result.matrix $2 $3 $4 $5
