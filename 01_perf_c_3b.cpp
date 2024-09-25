/*
Author: akashmaji(@iisc.ac.in)
File  : 01_perf_c_3b.cpp
*/

#include<stdio.h>
#include<stdlib.h>

#define ROWS 8192
#define COLS 8192
#define SIZE 8192

#define BLOCK_SIZE 64

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
            matrixC[row][col] = 0;
        }
    }


    if(matrixA == NULL || matrixB == NULL || matrixC == NULL){
        printf("Error: malloc() failed to allocate memory!\n");
        exit(0);
    }else{
        // printf("Success: memory allocated!\n");
    }

    // do matrix multiplication in a tiled fashion of tile-size=64
    // loop order: (j, i, k)
    for(int j = 0; j < COLS; j += BLOCK_SIZE){
        for(int i = 0; i < ROWS; i += BLOCK_SIZE){
            for(int k = 0; k < SIZE; k += BLOCK_SIZE){
                
                for(int jj = j; jj < j + BLOCK_SIZE; jj++){
                    for(int ii = i; ii < i + BLOCK_SIZE; ii++){
                        for(int kk = k; kk < k + BLOCK_SIZE; kk++){
                            matrixC[ii][jj] += (matrixA[ii][kk] * matrixB[kk][jj]);
                        }
                    }
                }

            }
        }
    }
    
    // printf("OK\n");

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
gcc 01_perf_c_3b.cpp -o 01_perf_c_3b

Run perf:
perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_c_3b

*/

/*------------SAMPLE-------------*/
/*

akash@victus:~/Desktop/HPCA$ perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_c_3b


 Performance counter stats for './01_perf_c_3b':

 2,108,483,723,398 ns   duration_time                                                         
 2,103,699,815,000 ns   user_time                                                             
       582,009,000 ns   system_time                                                           
   114,120,659,697      cache-references                                                        (41.67%)
    38,346,533,781      cache-misses                     #   33.60% of all cache refs           (41.67%)
    66,280,301,011      L1-dcache-load-misses                                                   (41.67%)
        25,262,379      L1-icache-load-misses                                                   (41.67%)
       413,226,451      dTLB-load-misses                                                        (41.67%)
   576,685,224,502      l1_dtlb_misses                                                          (41.67%)
       415,761,091      l2_dtlb_misses                                                          (41.67%)
28,178,327,988,613      instructions                     #    3.79  insn per cycle            
                                                  #    0.00  stalled cycles per insn     (41.67%)
 7,433,403,240,349      cpu-cycles                                                              (41.67%)
   108,346,358,472      stalled-cycles-frontend          #    1.46% frontend cycles idle        (41.67%)
   582,224,592,030      branch-instructions                                                     (41.67%)
     9,708,257,702      branch-misses                    #    1.67% of all branches             (41.67%)
           196,809      page-faults                                                           

    2108.483723398 seconds time elapsed

    2103.699815000 seconds user
       0.582009000 seconds sys


akash@victus:~/Desktop/HPCA$ 

*/
 