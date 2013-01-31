/*
 * multiplyMethods.c
 *
 *  Created on: Nov 12, 2012
 *      Author: parallels
 */
#include "multiplyMethods.h"
#include <omp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


Matrix dumbMatrixMultiply(Matrix a, Matrix b){
	int n = a.n;
	Matrix matrixResult = makeMatrix(n);

	for (int x = 0; x<n;++x){
		int y = 0;
		for (; y<n;++y){
			for (int i = 0; i<n;++i){
				matrixResult.arr[x][y]+= a.arr[x][i] * b.arr[i][y];
			}

		}
	}
	return matrixResult;
}

Matrix simpleMatrixMultiply(Matrix a, Matrix b){
	int n = a.n;
	Matrix matrixResult = makeMatrix(n);

	for (int x = 0; x<n;++x){
		int y = 0;
		for (; y<n;++y){
			/*x,y result matrix position, now calcs...*/
			int val = 0;

			for (int i = 0; i<n;++i){
				val+= a.arr[x][i] * b.arr[i][y];
			}

			matrixResult.arr[x][y] = val;
		}
	}
	return matrixResult;
}
Matrix advancedMatrixMultiply(Matrix a, Matrix b){
	int n = a.n;
	Matrix matrixResult = makeMatrix(n);

	int y = 0;
	for (; y<n;++y){
		int ii=0;
		int v[n];
		for (;ii<n;++ii){ v[ii]=b.arr[ii][y];}

		int x = 0;
		for (; x<n;++x){
			/*x,y result matrix position, now calcs...*/
			int val = 0;
			int i = 0;
			for (; i<n;++i){
				val+= a.arr[x][i] *v[i];/*v... see 1*/
			}

			matrixResult.arr[x][y] = val;
		}
	}
	return matrixResult;
}
Matrix optimizedMatrixMultiply(Matrix a, Matrix b){
	int n = a.n;
	Matrix matrixResult = makeMatrix(n);

	int y = 0;
	for (; y<n;++y){
		/*1 Placing b column in cache instead of getting it from the arr *ram) every time*/
		int ii=0;
		int v[n];
		for (;ii<n;++ii){ v[ii]=b.arr[ii][y];}

		int x = 0;
		for (; x<n;++x){
			/*for (; i<n;++i){
				val+= a.arr[x][i] *v[i];//v... see 1
			}*/
			int nN = n % 10;
			int topLimit = n - nN;

			int i = 0;
			int val = 0;

			if (topLimit>9){
				for (; i<topLimit; i+=10){   /*Loop unrolling*/
					val+= a.arr[x][i] *v[i];
					val+= a.arr[x][i+1] *v[i+1];
					val+= a.arr[x][i+2] *v[i+2];
					val+= a.arr[x][i+3] *v[i+3];
					val+= a.arr[x][i+4] *v[i+4];
					val+= a.arr[x][i+5] *v[i+5];
					val+= a.arr[x][i+6] *v[i+6];
					val+= a.arr[x][i+7] *v[i+7];
					val+= a.arr[x][i+8] *v[i+8];
					val+= a.arr[x][i+9] *v[i+9];
				}
			}

			i = topLimit;
			for(; i <topLimit+nN;++i){
				val+= a.arr[x][i] *v[i];
			}
			matrixResult.arr[x][y] = val;
		}
	}
	return matrixResult;
}


Matrix OMPdumbMatrixMultiply(Matrix a, Matrix b){
	int n = a.n;
	Matrix matrixResult = makeMatrix(n);

	#pragma omp parallel for
	for (int x = 0; x<n;++x){
		for (int y = 0; y<n;++y){
			for (int i = 0; i<n;++i){
				matrixResult.arr[x][y]+= a.arr[x][i] * b.arr[i][y];
			}

		}
	}
	return matrixResult;
}

Matrix OMPsimpleMatrixMultiply(Matrix a, Matrix b){
	int n = a.n;
	Matrix matrixResult = makeMatrix(n);

	#pragma omp parallel for
	for (int x = 0; x<n;++x){
		for (int y = 0; y<n;++y){
			/*x,y result matrix position, now calcs...*/

			int val = 0;
			for (int i = 0; i<n;++i){
				int var =  a.arr[x][i] * b.arr[i][y];
				val+=var;
			}

			matrixResult.arr[x][y] = val;
		}
	}
	return matrixResult;
}
Matrix OMPadvancedMatrixMultiply(Matrix a, Matrix b){
	int n = a.n;
	Matrix matrixResult = makeMatrix(n);

	#pragma omp parallel for  schedule(runtime)  shared(a,b,matrixResult)
	for (int y = 0; y<n;++y){

		int v[n];
		for (int ii=0;ii<n;++ii){ v[ii]=b.arr[ii][y];}

		for (int x = 0; x<n;++x){
			/*x,y result matrix position, now calcs...*/
			int val = 0;

			for (int i = 0; i<n;++i){
				val+= a.arr[x][i] *v[i];/*v... see 1*/
			}

			matrixResult.arr[x][y] = val;
		}
	}
	return matrixResult;
}
Matrix OMPoptimizedMatrixMultiply(Matrix a, Matrix b){
	int n = a.n;
	Matrix matrixResult = makeMatrix(n);

	#pragma omp parallel for
	for (int y = 0; y<n;++y){
		/*1 Placing b column in cache instead of getting it from the arr *ram) every time*/
		int ii=0;
		int v[n];
		for (;ii<n;++ii){ v[ii]=b.arr[ii][y];}

		int x = 0;
		for (; x<n;++x){
			/*for (; i<n;++i){
				val+= a.arr[x][i] *v[i];//v... see 1
			}*/
			int nN = n % 10;
			int topLimit = n - nN;

			int i = 0;
			int val = 0;

			if (topLimit>9){
				for (; i<topLimit; i+=10){   /*Loop unrolling*/
					val+= a.arr[x][i] *v[i];
					val+= a.arr[x][i+1] *v[i+1];
					val+= a.arr[x][i+2] *v[i+2];
					val+= a.arr[x][i+3] *v[i+3];
					val+= a.arr[x][i+4] *v[i+4];
					val+= a.arr[x][i+5] *v[i+5];
					val+= a.arr[x][i+6] *v[i+6];
					val+= a.arr[x][i+7] *v[i+7];
					val+= a.arr[x][i+8] *v[i+8];
					val+= a.arr[x][i+9] *v[i+9];
				}
			}

			i = topLimit;
			for(; i <topLimit+nN;++i){
				val+= a.arr[x][i] *v[i];
			}
			matrixResult.arr[x][y] = val;
		}
	}
	return matrixResult;
}


void *PTHREADdumbMatrixMultiplyrunner(void *param);
struct PTHREADdumbMatrixMultiplyRunnerParam{
	int x;
	int y;
	Matrix a;
	Matrix b;
	Matrix matrixResult;
	int n;
	int i;
};


void *PTHREADdumbMatrixMultiplyrunner(void *param);
Matrix PTHREADdumbMatrixMultiply(Matrix a, Matrix b){
	int n = a.n;
	Matrix matrixResult = makeMatrix(n);
	pthread_t* tId;
	tId = (pthread_t*) malloc(num_threads*sizeof(pthread_t));
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	for (int i = 0; i<num_threads;++i){
			struct PTHREADdumbMatrixMultiplyRunnerParam *data = (struct PTHREADdumbMatrixMultiplyRunnerParam *) malloc(sizeof(struct PTHREADdumbMatrixMultiplyRunnerParam));
			data->i = i;
			data->n = n;
			data->a = a;
			data->b = b;
			data->matrixResult = matrixResult;

			int err = pthread_create(&tId[i],&attr,PTHREADdumbMatrixMultiplyrunner,data);
			if (err!=0){
				exit(1);
			}
	}
	for (int i = 0; i<num_threads;i++)
		pthread_join(tId[i], NULL);

	return matrixResult;
}

void *PTHREADdumbMatrixMultiplyrunner(void *param){
	struct PTHREADdumbMatrixMultiplyRunnerParam *p = param;

	  int s = p->i;   // slice
	  int from = (s * p->n)/num_threads;
	  int to = ((s+1) * p->n)/num_threads;//rounding works to our needs here

	  //printf(" slice %d (from row %d to %d)\n", s, from, to-1);
	  for (int x = from; x < to; x++)
	  {
	    for (int y = 0; y < p->n; y++)
	    {
			for (int i = 0; i<p->n;++i){
				p->matrixResult.arr[x][y]+= p->a.arr[x][i] * p->b.arr[i][y];
			}
	    }
	  }
	pthread_exit(0);
	return 0;
}


void *PTHREADsimpleMatrixMultiplyRunner(void *param);
Matrix PTHREADsimpleMatrixMultiply(Matrix a, Matrix b){
	int n = a.n;
	Matrix matrixResult = makeMatrix(n);
	pthread_t* tId;
	tId = (pthread_t*) malloc(num_threads*sizeof(pthread_t));
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	for (int i = 0; i<num_threads;++i){
		struct PTHREADdumbMatrixMultiplyRunnerParam *data = (struct PTHREADdumbMatrixMultiplyRunnerParam *) malloc(sizeof(struct PTHREADdumbMatrixMultiplyRunnerParam));
		data->i = i;
		data->n = n;
		data->a = a;
		data->b = b;
		data->matrixResult = matrixResult;

		int err = pthread_create(&tId[i],&attr,PTHREADsimpleMatrixMultiplyRunner,data);
		if (err!=0){
			exit(1);
		}
}
for (int i = 0; i<num_threads;i++)
	pthread_join(tId[i], NULL);
	return matrixResult;
}

void *PTHREADsimpleMatrixMultiplyRunner(void *param){
	struct PTHREADdumbMatrixMultiplyRunnerParam *p = param;

	  int s = p->i;   // slice
	  int from = (s * p->n)/num_threads;
	  int to = ((s+1) * p->n)/num_threads;//rounding works to our needs here

	  //printf(" slice %d (from row %d to %d)\n", s, from, to-1);
	  for (int x = from; x < to; x++)
	  {
	    for (int y = 0; y < p->n; y++)
	    {
	    	int val = 0;
			for (int i = 0; i<p->n;++i){
				val+= p->a.arr[x][i] * p->b.arr[i][y];
			}
			p->matrixResult.arr[x][y] = val;
	    }
	  }
	pthread_exit(0);
	return 0;
}

void *PTHREADadvancedMatrixMultiplyRunner(void *param);
Matrix PTHREADadvancedMatrixMultiply(Matrix a, Matrix b){
	int n = a.n;
	Matrix matrixResult = makeMatrix(n);
	pthread_t* tId;
	tId = (pthread_t*) malloc(num_threads*sizeof(pthread_t));
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	for (int i = 0; i<num_threads;++i){
		struct PTHREADdumbMatrixMultiplyRunnerParam *data = (struct PTHREADdumbMatrixMultiplyRunnerParam *) malloc(sizeof(struct PTHREADdumbMatrixMultiplyRunnerParam));
		data->i = i;
		data->n = n;
		data->a = a;
		data->b = b;
		data->matrixResult = matrixResult;

		int err = pthread_create(&tId[i],&attr,PTHREADadvancedMatrixMultiplyRunner,data);
		if (err!=0){
			exit(1);
		}
}
for (int i = 0; i<num_threads;i++)
	pthread_join(tId[i], NULL);
	return matrixResult;
}

void *PTHREADadvancedMatrixMultiplyRunner(void *param){
	struct PTHREADdumbMatrixMultiplyRunnerParam *p = param;

	  int s = p->i;   // slice
	  int from = (s * p->n)/num_threads;
	  int to = ((s+1) * p->n)/num_threads;//rounding works to our needs here

	  //printf(" slice %d (from row %d to %d)\n", s, from, to-1);
	  for (int y = from; y < to; y++)
	  {
		int v[p->n];
		for (int ii=0;ii<p->n;++ii){ v[ii]=p->b.arr[ii][y];}

	    for (int x = 0; x < p->n; x++)
	    {
	    	int val = 0;
			for (int i = 0; i<p->n;++i){
				val+= p->a.arr[x][i] * v[i];
			}
			p->matrixResult.arr[x][y] = val;
	    }
	  }
	pthread_exit(0);
	return 0;
}


void *PTHREADoptimizedMatrixMultiplyRunner(void *param);
Matrix PTHREADoptimizedMatrixMultiply(Matrix a, Matrix b){
	int n = a.n;
	Matrix matrixResult = makeMatrix(n);
	pthread_t* tId;
	tId = (pthread_t*) malloc(num_threads*sizeof(pthread_t));
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	for (int i = 0; i<num_threads;++i){
		struct PTHREADdumbMatrixMultiplyRunnerParam *data = (struct PTHREADdumbMatrixMultiplyRunnerParam *) malloc(sizeof(struct PTHREADdumbMatrixMultiplyRunnerParam));
		data->i = i;
		data->n = n;
		data->a = a;
		data->b = b;
		data->matrixResult = matrixResult;

		int err = pthread_create(&tId[i],&attr,PTHREADoptimizedMatrixMultiplyRunner,data);
		if (err!=0){
			exit(1);
		}
}
for (int i = 0; i<num_threads;i++)
	pthread_join(tId[i], NULL);
	return matrixResult;
}

void *PTHREADoptimizedMatrixMultiplyRunner(void *param){
	struct PTHREADdumbMatrixMultiplyRunnerParam *p = param;

	  int s = p->i;   // slice
	  int from = (s * p->n)/num_threads;
	  int to = ((s+1) * p->n)/num_threads;//rounding works to our needs here

	  //printf(" slice %d (from row %d to %d)\n", s, from, to-1);
	  for (int y = from; y < to; y++)
	  {
		int v[p->n];
		for (int ii=0;ii<p->n;++ii){ v[ii]=p->b.arr[ii][y];}

	    for (int x = 0; x < p->n; x++)
	    {
	    	int val = 0;
			int nN = p->n % 10;
			int topLimit = p->n - nN;
			int i = 0;
			if (topLimit>9){
				for (i = 0; i<p->n;++i){
					val+= p->a.arr[x][i] *v[i];
					val+= p->a.arr[x][i+1] *v[i+1];
					val+= p->a.arr[x][i+2] *v[i+2];
					val+= p->a.arr[x][i+3] *v[i+3];
					val+= p->a.arr[x][i+4] *v[i+4];
					val+= p->a.arr[x][i+5] *v[i+5];
					val+= p->a.arr[x][i+6] *v[i+6];
					val+= p->a.arr[x][i+7] *v[i+7];
					val+= p->a.arr[x][i+8] *v[i+8];
					val+= p->a.arr[x][i+9] *v[i+9];
				}
			}
			i = topLimit;
			for(; i <topLimit+nN;++i){
				val+= p->a.arr[x][i] *v[i];
			}
			p->matrixResult.arr[x][y] = val;
	    }
	  }
	pthread_exit(0);
	return 0;
}










Matrix MPIdumbMatrixMultiply(Matrix a, Matrix b){
	int n = a.n;
	Matrix matrixResult = makeMatrix(n);
	int s = mpiRank;   // slice
	int from = (s * n)/mpiSize;
	int to = ((s+1) * n)/mpiSize;//rounding works to our needs here
	printf(" slice %d (from row %d to %d)\n", s, from, to-1);

	for (int x = from; x < to; x++)
	{
		for (int y = 0; y < n; y++)
		{
				for (int i = 0; i<n;++i){
					matrixResult.arr[x][y]+= a.arr[x][i] * b.arr[i][y];
				}
		}
	}

	if (mpiRank >0){
		MPI_Send(&(matrixResult.arr[from][0]), n*(to-from)*sizeof(int), MPI_BYTE,0,42,MPI_COMM_WORLD);
	}else{
		for(int i = 1; i<mpiSize;i++){
			from = (i * n)/mpiSize;
			to = ((i+1) * n)/mpiSize;//rounding works to our needs here
			MPI_Recv(&(matrixResult.arr[from][0]),n*(to-from)*sizeof(int),MPI_BYTE,i,42,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
	}
	return matrixResult;
}








Matrix MPIsimpleMatrixMultiply(Matrix a, Matrix b){
	int n = a.n;
	Matrix matrixResult = makeMatrix(n);
	int s = mpiRank;   // slice
	int from = (s * n)/mpiSize;
	int to = ((s+1) * n)/mpiSize;//rounding works to our needs here
	printf(" slice %d (from row %d to %d)\n", s, from, to-1);

	  for (int x = from; x < to; x++)
	  {
	    for (int y = 0; y < n; y++)
	    {
	    	int val = 0;
			for (int i = 0; i<n;++i){
				val+= a.arr[x][i] * b.arr[i][y];
			}
			matrixResult.arr[x][y] = val;
	    }
	  }

	if (mpiRank >0){
		MPI_Send(&(matrixResult.arr[from][0]), n*(to-from)*sizeof(int), MPI_BYTE,0,42,MPI_COMM_WORLD);
	}else{
		for(int i = 1; i<mpiSize;i++){
			from = (i * n)/mpiSize;
			to = ((i+1) * n)/mpiSize;//rounding works to our needs here
			MPI_Recv(&(matrixResult.arr[from][0]),n*(to-from)*sizeof(int),MPI_BYTE,i,42,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
	}
	return matrixResult;
}
Matrix MPIadvancedMatrixMultiply(Matrix a, Matrix b){
	int n = a.n;
	Matrix matrixResult = makeMatrix(n);
	int s = mpiRank;   // slice
	int from = (s * n)/mpiSize;
	int to = ((s+1) * n)/mpiSize;//rounding works to our needs here
	printf(" slice %d (from row %d to %d)\n", s, from, to-1);

	  for (int y = 0; y < n; y++)
	  {
		int v[n];
		for (int ii=0;ii<n;++ii){ v[ii]=b.arr[ii][y];}

	    for (int x = from; x < to; x++)
	    {
	    	int val = 0;
			for (int i = 0; i<n;++i){
				val+= a.arr[x][i] * v[i];
			}
			matrixResult.arr[x][y] = val;
	    }
	  }

	if (mpiRank >0){
		MPI_Send(&(matrixResult.arr[from][0]), n*(to-from)*sizeof(int), MPI_BYTE,0,42,MPI_COMM_WORLD);
	}else{
		for(int i = 1; i<mpiSize;i++){
			from = (i * n)/mpiSize;
			to = ((i+1) * n)/mpiSize;//rounding works to our needs here
			MPI_Recv(&(matrixResult.arr[from][0]),n*(to-from)*sizeof(int),MPI_BYTE,i,42,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
	}
	return matrixResult;
}
Matrix MPIoptimizedMatrixMultiply(Matrix a, Matrix b){
	int n = a.n;
	Matrix matrixResult = makeMatrix(n);
	int s = mpiRank;   // slice
	int from = (s * n)/mpiSize;
	int to = ((s+1) * n)/mpiSize;//rounding works to our needs here
	printf(" slice %d (from row %d to %d)\n", s, from, to-1);

	  for (int y = from; y < to; y++)
	  {
		int v[n];
		for (int ii=0;ii<n;++ii){ v[ii]=b.arr[ii][y];}

	    for (int x = 0; x < n; x++)
	    {
	    	int val = 0;
			int nN = n % 10;
			int topLimit = n - nN;
			int i = 0;
			if (topLimit>9){
				for (i = 0; i<n;++i){
					val+= a.arr[x][i] *v[i];
					val+= a.arr[x][i+1] *v[i+1];
					val+= a.arr[x][i+2] *v[i+2];
					val+= a.arr[x][i+3] *v[i+3];
					val+= a.arr[x][i+4] *v[i+4];
					val+= a.arr[x][i+5] *v[i+5];
					val+= a.arr[x][i+6] *v[i+6];
					val+= a.arr[x][i+7] *v[i+7];
					val+= a.arr[x][i+8] *v[i+8];
					val+= a.arr[x][i+9] *v[i+9];
				}
			}
			i = topLimit;
			for(; i <topLimit+nN;++i){
				val+= a.arr[x][i] *v[i];
			}
			matrixResult.arr[x][y] = val;
	    }
	  }

	if (mpiRank >0){
		MPI_Send(&(matrixResult.arr[from][0]), n*(to-from)*sizeof(int), MPI_BYTE,0,42,MPI_COMM_WORLD);
	}else{
		for(int i = 1; i<mpiSize;i++){
			from = (i * n)/mpiSize;
			to = ((i+1) * n)/mpiSize;//rounding works to our needs here
			MPI_Recv(&(matrixResult.arr[from][0]),n*(to-from)*sizeof(int),MPI_BYTE,i,42,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
	}
	return matrixResult;
}
