/*
Author: akashmaji(@iisc.ac.in)
File:   01_perf_a_1.cpp
*/

#include<stdio.h>
#include<stdlib.h>

#define ROWS 2048
#define COLS 2048
#define SIZE 2048

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
gcc 01_perf_a_1.cpp -o 01_perf_a_1

Run perf:
perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_a_1

*/

/*------------SAMPLE-------------*/
/*

akash@victus:~/Desktop/HPCA$ gcc 01_perf_a_1.cpp -o 01_perf_a_1
akash@victus:~/Desktop/HPCA$ perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_a_1

 Performance counter stats for './01_perf_a_1':

    32,431,597,943 ns   duration_time                                                         
    32,407,530,000 ns   user_time                                                             
        22,999,000 ns   system_time                                                           
    28,425,720,751      cache-references                                                        (41.66%)
    10,697,923,037      cache-misses                     #   37.63% of all cache refs           (41.66%)
    10,386,290,511      L1-dcache-load-misses                                                   (41.67%)
           329,753      L1-icache-load-misses                                                   (41.67%)
     5,751,002,472      dTLB-load-misses                                                        (41.67%)
     8,698,414,482      l1_dtlb_misses                                                          (41.67%)
     5,740,368,114      l2_dtlb_misses                                                          (41.67%)
   421,870,370,482      instructions                     #    3.05  insn per cycle            
                                                  #    0.00  stalled cycles per insn     (41.67%)
   138,275,027,686      cpu-cycles                                                              (41.67%)
       359,895,654      stalled-cycles-frontend          #    0.26% frontend cycles idle        (41.67%)
     8,807,025,680      branch-instructions                                                     (41.66%)
        13,943,978      branch-misses                    #    0.16% of all branches             (41.66%)
            12,378      page-faults                                                           

      32.431597943 seconds time elapsed

      32.407530000 seconds user
       0.022999000 seconds sys


akash@victus:~/Desktop/HPCA$ 

*/
/*-------------------------*/