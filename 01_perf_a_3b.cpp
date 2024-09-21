/*
Author: akashmaji(@iisc.ac.in)
File:   01_perf_a_3.cpp
*/

#include<stdio.h>
#include<stdlib.h>

#define ROWS 8192
#define COLS 8192
#define SIZE 8192

#define LIMIT 1000

void print_matrix(int** matrix, int N, int M){
    for(int row = 0; row < N; row++){
        for(int col = 0; col < M; col++){
            printf("%d ", matrix[row][col]);
        }
        printf("\n");
    }
}

int main(){

    // allocate memory dynamically to matrices A, B and C
    int **matrixA = (int**)malloc(ROWS * sizeof(int*));
    int **matrixB = (int**)malloc(ROWS * sizeof(int*));
    int **matrixC = (int**)malloc(ROWS * sizeof(int*));
    for(int row = 0; row < ROWS; row++){
        matrixA[row] = (int*)malloc(COLS * sizeof(int));
        matrixB[row] = (int*)malloc(COLS * sizeof(int));
        matrixC[row] = (int*)malloc(COLS * sizeof(int));
    }

    // initialize matrices A and B with some random values within 'LIMIT'
    for(int row = 0; row < ROWS; row++){
        for(int col = 0; col < COLS; col++){
            matrixA[row][col] = rand() % LIMIT;
            matrixB[row][col] = rand() % LIMIT;
        }
    }

    // initialize A, B to zeros
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
             matrixC[i][j] = 0;
        }
    }
    
    // do the matrix multiplication C = A * B
    // loop order is (k, i, j)
    for(int k = 0; k < SIZE; k++){
        for(int i = 0; i < ROWS; i++){
            for(int j = 0; j < COLS; j++){
                matrixC[i][j] += (matrixA[i][k] * matrixB[k][j]);
            }
        }
    }

    // see matrix C
    // print_matrix(matrixC, ROWS, COLS);

    // memory cleanup for malloced matrices A and B
    for(int row = 0; row < ROWS; row++){
        free(matrixA[row]);
        free(matrixB[row]);
    }
    free(matrixA);
    free(matrixB);

    return 0;
}

/*--------Instructions-------------*/
/*

Compile:
gcc 01_perf_a_3b.cpp -o 01_perf_a_3b

Run perf:
perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_a_3b

*/

/*------------SAMPLE-------------*/
/*

akash@victus:~/Desktop/HPCA$ perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_a_3b

 Performance counter stats for './01_perf_a_3b':

 1,634,661,242,075 ns   duration_time                                                         
 1,630,848,939,000 ns   user_time                                                             
       353,006,000 ns   system_time                                                           
   113,121,339,432      cache-references                                                        (41.67%)
     4,848,749,846      cache-misses                     #    4.29% of all cache refs           (41.67%)
    70,360,391,174      L1-dcache-load-misses                                                   (41.67%)
        10,579,402      L1-icache-load-misses                                                   (41.67%)
       663,254,045      dTLB-load-misses                                                        (41.67%)
       768,621,548      l1_dtlb_misses                                                          (41.67%)
       663,391,711      l2_dtlb_misses                                                          (41.67%)
26,975,539,185,199      instructions                     #    4.36  insn per cycle            
                                                  #    0.00  stalled cycles per insn     (41.67%)
 6,182,075,718,231      cpu-cycles                                                              (41.67%)
    20,861,597,707      stalled-cycles-frontend          #    0.34% frontend cycles idle        (41.67%)
   555,243,674,614      branch-instructions                                                     (41.67%)
       528,193,488      branch-misses                    #    0.10% of all branches             (41.67%)
           196,807      page-faults                                                           

    1634.661242075 seconds time elapsed

    1630.848939000 seconds user
       0.353006000 seconds sys


akash@victus:~/Desktop/HPCA$

*/
/*-------------------------*/