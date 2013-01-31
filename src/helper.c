/*
 * helper.c
 *
 *  Created on: Nov 11, 2012
 *      Author: parallels
 */

#include "helper.h"
#include <stdio.h>
#include <stdlib.h>

Matrix makeMatrix(int n){
	int* matrixM = calloc(n*n,sizeof(int));
	int** matrixPointer = malloc(n*sizeof(int*));

	int i = 0;
	for (;i<n;++i){
		matrixPointer[i] = matrixM + i*n;
	}
	Matrix m;
	m.arr = matrixPointer;
	m.n = n;
	m.m = matrixM;
	return m;
}

Matrix readMatrixFromFile(char * fileName){
	FILE * f = fopen(fileName,"r");
	if (f==NULL){
		printf("Error opening File %s\n",fileName);
		exit(1);
	}
	int n;
	int err = fscanf(f,"%d", &n);
	if (err<0){
		puts("Something wrong happened during parsing a matrix file. Maybe a formatting error?");
		exit(1);
	}
	Matrix matrix = makeMatrix(n);

	int x = 0;
	for(;x<n;++x){
		int y = 0;
		for(;y<n;++y){
			err = fscanf(f,"%d",&matrix.arr[x][y]);
			if (err < 0){
				puts("Something wrong happened during parsing a matrix file. Maybe a formatting error?");
				exit(1);
			}
		}
	}

	fclose(f);

	return matrix;
}

void printMatrix(Matrix m){
	int n = m.n;
	int x = 0;
	for(;x<n;++x){
		int y = 0;
		for(;y<n;++y){
			printf("\t%d",m.arr[x][y]);
		}
		printf("\n");
	}}

void checkMOps(Matrix a,Matrix b){
	if (a.n == b.n) printf("Operands are correctly sized\n");
	else{
		puts("Operands are not of the same size");
		exit(1);
	}
}

void freeMatrix(Matrix m){
	free(*(m.arr));
	free(m.arr);
}

void checkResult(Matrix a,Matrix b){
	int n = a.n;
	int x = 0;
	for (; x<n;++x){
		int y = 0;
		for (; y<n;++y){
			if(a.arr[x][y]!=b.arr[x][y]){
				puts("Result differs from expected result!");
				printf("At x%d y%d with a=%d and b=%d \n",x,y,a.arr[x][y],b.arr[x][y]);

				exit(1);
			}
		}
	}
	puts("Result complies with the expected result!");
}

void writeTimeToFile(int n,int opt, long time){
	char buffer[32];
	sprintf(buffer,"performance_n_%d_c_%d",n,opt);
	FILE* f = fopen(buffer,"a+");
	fprintf(f,"%ld\n",time);
	fclose(f);
}
