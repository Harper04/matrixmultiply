/*
 * multiplyMethods.h
 *
 *  Created on: Nov 12, 2012
 *      Author: parallels
 */

#ifndef MULTIPLYMETHODS_H_
#define MULTIPLYMETHODS_H_

#include "helper.h"
extern int num_threads;
extern int mpiRank;
extern int mpiSize;
Matrix dumbMatrixMultiply(Matrix a, Matrix b);
Matrix simpleMatrixMultiply(Matrix a, Matrix b);
Matrix advancedMatrixMultiply(Matrix a, Matrix b);
Matrix optimizedMatrixMultiply(Matrix a, Matrix b);

Matrix OMPdumbMatrixMultiply(Matrix a, Matrix b);
Matrix OMPsimpleMatrixMultiply(Matrix a, Matrix b);
Matrix OMPadvancedMatrixMultiply(Matrix a, Matrix b);
Matrix OMPoptimizedMatrixMultiply(Matrix a, Matrix b);

Matrix PTHREADdumbMatrixMultiply(Matrix a, Matrix b);
Matrix PTHREADsimpleMatrixMultiply(Matrix a, Matrix b);
Matrix PTHREADadvancedMatrixMultiply(Matrix a, Matrix b);

Matrix MPIdumbMatrixMultiply(Matrix a, Matrix b);
Matrix MPIsimpleMatrixMultiply(Matrix a, Matrix b);
Matrix MPIadvancedMatrixMultiply(Matrix a, Matrix b);

#endif /* MULTIPLYMETHODS_H_ */
