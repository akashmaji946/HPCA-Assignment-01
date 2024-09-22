/*
Author: akashmaji(@iisc.ac.in)
File  : 01_perf_c_1b.cpp
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
    // loop order: (i, j, k)
    for(int i = 0; i < ROWS; i += BLOCK_SIZE){
            for(int j = 0; j < COLS; j += BLOCK_SIZE){
                for(int k = 0; k < SIZE; k += BLOCK_SIZE){
                
                    for(int ii = i; ii < i + BLOCK_SIZE; ii++){
                        for(int jj = j; jj < j + BLOCK_SIZE; jj++){
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
gcc 01_perf_c_1b.cpp -o 01_perf_c_1b

Run perf:
perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_c_1b

*/

/*------------SAMPLE-------------*/
/*

akash@victus:~/Desktop/HPCA$ perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_c_1b



 Performance counter stats for './01_perf_c_1b':

 2,091,190,059,607 ns   duration_time                                                         
 2,086,445,759,000 ns   user_time                                                             
       578,006,000 ns   system_time                                                           
    40,794,357,647      cache-references                                                        (41.67%)
    17,730,019,170      cache-misses                     #   43.46% of all cache refs           (41.67%)
    19,350,870,716      L1-dcache-load-misses                                                   (41.67%)
        27,542,605      L1-icache-load-misses                                                   (41.67%)
       321,838,788      dTLB-load-misses                                                        (41.67%)
   484,377,408,805      l1_dtlb_misses                                                          (41.67%)
       322,144,886      l2_dtlb_misses                                                          (41.67%)
28,175,612,828,613      instructions                     #    3.83  insn per cycle            
                                                  #    0.00  stalled cycles per insn     (41.67%)
 7,362,232,138,080      cpu-cycles                                                              (41.67%)
   109,440,396,214      stalled-cycles-frontend          #    1.49% frontend cycles idle        (41.67%)
   582,334,403,417      branch-instructions                                                     (41.67%)
     9,683,894,724      branch-misses                    #    1.66% of all branches             (41.67%)
           196,807      page-faults                                                           

    2091.190059607 seconds time elapsed

    2086.445759000 seconds user
       0.578006000 seconds sys


akash@victus:~/Desktop/HPCA$ 

*/
 