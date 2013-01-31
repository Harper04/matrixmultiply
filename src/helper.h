/*
 * helper.h
 *
 *  Created on: Nov 11, 2012
 *      Author: parallels
 */

#ifndef HELPER_H_
#define HELPER_H_

typedef struct {
	int n;
	int ** arr;
	int * m;
} Matrix;

typedef enum { false, true } bool;

Matrix makeMatrix(int n);
Matrix readMatrixFromFile(char* FILE);
void printMatrix(Matrix arr);
void checkMOps(Matrix a,Matrix b);
void freeMatrix(Matrix m);
void checkResult(Matrix a,Matrix b);
void writeTimeToFile(int n,int opt, long time);
#endif /* HELPER_H_ */
