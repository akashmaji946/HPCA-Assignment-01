/*
Author: akashmaji(@iisc.ac.in)
File:   01_perf_a_1b.cpp
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
    // loop order is (i, j, k)
        for(int i = 0; i < ROWS; i++){
            for(int j = 0; j < COLS; j++){
                for(int k = 0; k < SIZE; k++){
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
gcc 01_perf_a_1b.cpp -o 01_perf_a_1b

Run perf:
perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_a_1b

*/

/*------------SAMPLE-------------*/
/*

akash@victus:~/Desktop/HPCA$ perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_a_1b

 Performance counter stats for './01_perf_a_1b':

 3,024,660,933,510 ns   duration_time                                                         
 3,020,189,251,000 ns   user_time                                                             
       594,989,000 ns   system_time                                                           
 2,035,190,150,439      cache-references                                                        (41.67%)
 1,144,956,746,884      cache-misses                     #   56.26% of all cache refs           (41.67%)
   663,457,112,610      L1-dcache-load-misses                                                   (41.67%)
       116,921,416      L1-icache-load-misses                                                   (41.67%)
   553,341,465,592      dTLB-load-misses                                                        (41.67%)
   555,289,780,973      l1_dtlb_misses                                                          (41.67%)
   553,517,963,900      l2_dtlb_misses                                                          (41.67%)
26,985,777,004,860      instructions                     #    2.32  insn per cycle            
                                                  #    0.00  stalled cycles per insn     (41.67%)
11,650,960,069,750      cpu-cycles                                                              (41.67%)
    75,515,803,707      stalled-cycles-frontend          #    0.65% frontend cycles idle        (41.67%)
   558,280,968,508      branch-instructions                                                     (41.67%)
       886,694,265      branch-misses                    #    0.16% of all branches             (41.67%)
           196,806      page-faults                                                           

    3024.660933510 seconds time elapsed

    3020.189251000 seconds user
       0.594989000 seconds sys


akash@victus:~/Desktop/HPCA$ 

*/
/*-------------------------*/