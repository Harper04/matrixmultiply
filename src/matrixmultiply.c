/*
 ============================================================================
 Name        : matrixmultiply.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "helper.h"
#include "multiplyMethods.h"
#include "sys/time.h"
#include "string.h"
#include <omp.h>
#include <mpi.h>

int num_threads = 8;
int mpiRank = 0, mpiSize = 1;

int main(int argc, char *argv[]) {

	if (argc == 1){
		printf("Usage:\n");
		printf("matrixmultiply fileNameOperandA fileNameOperandB fileNameExpectedResult [-p] [-o$c]:\n");
		printf("-p Will print the input and result matrices:\n");
		printf("-o Lets you choose the multiply method (level of optimization. Valid values for $c are:)\n");
		printf("\t0 dumbMatrixMultiply - most naive Way to multiply 2 matrices with no optimizations\n");
		printf("\t1 simpleMatrixMultiply - uses a register instead of adding the sum into ram\n");
		printf("\t2 advancedMultiply - t1 plus saving column of operandB in a temp array. -> better caching\n");
		printf("\t3 optimizedMultiply - t2 plus dynamic loop unrolling.\n\t\t seems to be slower then t2 if compiled with -O3 enabled. Better with big matrices\n");
		printf("\t4-7 Just o0-o3 with OpenMP\n");
		printf("\t8-11 Just o0-o3 with pThreads. Work is sliced based on the lines of the x-axes and the number of threads\n");
		printf("\t12-15 Just o0-o3 with MPI. Work is sliced based on the lines of the x-axes and the count of nodes\n");
		printf("-t Lets you choose the number of threads for multithreaded processing\n");

	}

	int calc = 3; /*Best is default*/


	bool print = false;

	for(int i=1;i<argc;++i){
		if( strcmp( argv[i], "-p") == 0)
			print = true;
		if( strcmp( argv[i], "-o") >= 0)
			sscanf(argv[i],"-o%d", &calc);
		if( strcmp( argv[i], "-t") >= 0){
			sscanf(argv[i],"-t%d", &num_threads);
		}
	}


	if ((calc > 11 && calc < 16 ) || argc==0){
		//Okay we want MPI, or mayby we are just a slave so lets connect to mpi
		//slaves should be started with -o12/13/14/15 too so they'll execute this
	    MPI_Init(0,0);
	    MPI_Comm_rank(MPI_COMM_WORLD, &mpiRank);
	    MPI_Comm_size(MPI_COMM_WORLD, &mpiSize);

	    //for slaves without arg and just to make sure we got everything in sync
		MPI_Bcast(&calc, 1, MPI_INT, 0, MPI_COMM_WORLD);

	}

	Matrix matrixA;
	Matrix matrixB;
	Matrix matrixExpectedResult;
	if (mpiRank == 0){
		//MPI non root processes shouldnt do this, but everyone else including non mpi

		if (argc < 4 || argc > 7 ) {
			/*We need 3 Filenames as Arguments*/
			puts("Please provide 3 files (names) containing 2 matrixes (operands) and the expected solution (for verification) as arguments");
			exit(1);
		}

		//OMP only if we want OMP
		if (calc > 3 && calc < 8)
			omp_set_num_threads(num_threads);

		matrixA = readMatrixFromFile(argv[1]);
		matrixB = readMatrixFromFile(argv[2]);
		matrixExpectedResult = readMatrixFromFile(argv[3]);


		if (print == true){
			printf("Input Matrix A (%s):\n",argv[1]);
			printMatrix(matrixA);
			printf("Input Matrix B (%s):\n",argv[2]);
			printMatrix(matrixB);
			printf("Input expected Matrixsolution (%s):\n",argv[3]);
			printMatrix(matrixExpectedResult);
		}
		/*A and B have to be the same size*/
		checkMOps(matrixA,matrixB);
		if (mpiSize > 1)
			MPI_Bcast(&matrixA.n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	}else{
		//only MPI processes with rank > 0
		int n = 0;
		MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
		matrixA = makeMatrix(n);
		matrixB = makeMatrix(n);
	}

	if (mpiSize > 1){
		//only when using MPI
		MPI_Bcast(matrixA.m,matrixA.n*matrixA.n*sizeof(int),MPI_BYTE,0,MPI_COMM_WORLD);
		MPI_Bcast(matrixB.m,matrixB.n*matrixB.n*sizeof(int),MPI_BYTE,0,MPI_COMM_WORLD);
	}

	struct timeval t0;
	struct timeval t1;
	if(mpiRank == 0){
		/*Our calculation begins
		Take Time*/
		gettimeofday(&t0, 0);
	}

	Matrix matrixResult;
	switch(calc){
		case 0:
			matrixResult = dumbMatrixMultiply(matrixA,matrixB);
			break;
		case 1:
			matrixResult = simpleMatrixMultiply(matrixA,matrixB);
			break;
		case 2:
			matrixResult = advancedMatrixMultiply(matrixA,matrixB);
			break;
		case 3:
			matrixResult = optimizedMatrixMultiply(matrixA,matrixB);
			break;
		case 4:
			matrixResult = OMPdumbMatrixMultiply(matrixA,matrixB);
			break;
		case 5:
			matrixResult = OMPsimpleMatrixMultiply(matrixA,matrixB);
			break;
		case 6:
			matrixResult = OMPadvancedMatrixMultiply(matrixA,matrixB);
			break;
		case 7:
			matrixResult = OMPoptimizedMatrixMultiply(matrixA,matrixB);
			break;
		case 8:
			matrixResult = PTHREADdumbMatrixMultiply(matrixA,matrixB);
			break;
		case 9:
			matrixResult = PTHREADsimpleMatrixMultiply(matrixA,matrixB);
			break;
		case 10:
			matrixResult = PTHREADadvancedMatrixMultiply(matrixA,matrixB);
			break;
		case 11:
			matrixResult = PTHREADadvancedMatrixMultiply(matrixA,matrixB);
			break;
		case 12:
			matrixResult = MPIdumbMatrixMultiply(matrixA,matrixB);
			break;
		case 13:
			matrixResult = MPIsimpleMatrixMultiply(matrixA,matrixB);
			break;
		case 14:
			matrixResult = MPIadvancedMatrixMultiply(matrixA,matrixB);
			break;
		case 15:
			matrixResult = MPIadvancedMatrixMultiply(matrixA,matrixB);
			break;
		default:
			matrixResult = optimizedMatrixMultiply(matrixA,matrixB);
			break;
	}




	if (mpiRank == 0){
		/*Calculation Ends here
		Take Time*/
		gettimeofday(&t1, 0);
		long elapsedTime = (t1.tv_sec-t0.tv_sec)*1000000 + t1.tv_usec-t0.tv_usec;
		if (print == true){
			printf("Resultmatrix:\n");
			printMatrix(matrixResult);
		}
		checkResult(matrixExpectedResult,matrixResult);
		printf("Microseconds needed: %ld\n",elapsedTime);
		writeTimeToFile(matrixA.n,calc,elapsedTime);
	}

	freeMatrix(matrixA);
	freeMatrix(matrixB);
	freeMatrix(matrixResult);
	if (mpiRank == 0){
		freeMatrix(matrixExpectedResult);
	}
	if (mpiSize > 1)
		MPI_Finalize();
	return EXIT_SUCCESS;
}
