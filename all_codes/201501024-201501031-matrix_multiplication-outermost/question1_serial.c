#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define CLK CLOCK_MONOTONIC

struct timespec diff(struct timespec start, struct timespec end){
	struct timespec temp;
	if((end.tv_nsec-start.tv_nsec)<0){
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	}
	else{
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}


int main(int argc, char* argv[]) {

	struct timespec start_e2e, end_e2e, start_alg, end_alg, e2e, alg;
	clock_gettime(CLK, &start_e2e);

	/* Check if enough command-line arguments are taken in. */
	if(argc < 3){
		printf( "Usage: %s n p \n", argv[0] );
		return -1;
	}

	int n = atoi(argv[1]);			/* size of input array */
	int p = atoi(argv[2]);			/* number of processors*/
	char *problem_name = "matrix_multiplication";
	char *approach_name = "outermost";

	FILE* outputFile;
	char outputFileName[100];
	sprintf(outputFileName, "output/%s_%s_%s_%s_output.txt", problem_name, approach_name, argv[1], argv[2]);


	int* matrix1[n];	// First operand  (matrix of size nxn)
	int* matrix2[n];	// Second operand (matrix of size nxn)
	int* mmAnswer[n]; 	// Answer of matrix multiplication is to be stored in this matrix (matrix of size nxn)
	int i, j, k;

	// Allocating space to matrix of nxn
	for(i = 0; i < n; i++) {
		matrix1[i] = malloc(sizeof(int) * n);
		matrix2[i] = malloc(sizeof(int) * n);
		mmAnswer[i] = malloc(sizeof(int) * n);
	}
	
	// Initializing all entries by 1
	for(i = 0; i < n; i++) {
		for(j = 0; j < n; j++) {
			matrix1[i][j] = 1;
			matrix2[i][j] = 1;
			mmAnswer[i][j] = 0;
		}
	}

	int number_of_threads = p;
	omp_set_num_threads(number_of_threads);
	
	clock_gettime(CLK, &start_alg); 		/* Start the algo timer */
	
	// Serial Execution
	
	for(i = 0; i < n; i++) {
		for(j = 0; j < n; j++) {
			for(k = 0; k < n; k++) {
				mmAnswer[i][j] += (matrix1[i][k] * matrix2[k][j]);
			}
		}
	}
	
	clock_gettime(CLK, &end_alg); 	/* End the algo timer */
	/*
	for(i = 0; i < n; i++) {
        	for(j = 0; j < n; j++)
                	printf("%d ", mmAnswer[i][j]);
                printf("\n");
        }
	*/
	clock_gettime(CLK, &end_e2e);
	e2e = diff(start_e2e, end_e2e);
	alg = diff(start_alg, end_alg);

	outputFile = fopen(outputFileName,"w");
	printf("%s,%s,%d,%d,%d,%ld,%d,%ld\n", problem_name, approach_name, n, p, e2e.tv_sec, e2e.tv_nsec, alg.tv_sec, alg.tv_nsec);
	return 0;
}
