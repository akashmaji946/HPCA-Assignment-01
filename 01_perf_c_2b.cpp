/*
Author: akashmaji(@iisc.ac.in)
File  : 01_perf_c_2b.cpp
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
    // loop order: (k, i, j)
    for(int k = 0; k < SIZE; k += BLOCK_SIZE){
        for(int i = 0; i < ROWS; i += BLOCK_SIZE){
            for(int j = 0; j < COLS; j += BLOCK_SIZE){
                
                for(int kk = k; kk < k + BLOCK_SIZE; kk++){
                    for(int ii = i; ii < i + BLOCK_SIZE; ii++){
                        for(int jj = j; jj < j + BLOCK_SIZE; jj++){
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
gcc 01_perf_c_2b.cpp -o 01_perf_c_2b

Run perf:
perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_c_2b

*/

/*------------SAMPLE-------------*/
/*
 
akash@victus:~/Desktop/HPCA$ perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_c_2b


 Performance counter stats for './01_perf_c_2b':

 2,063,665,620,797 ns   duration_time                                                         
 2,059,293,776,000 ns   user_time                                                             
       484,997,000 ns   system_time                                                           
    85,600,226,129      cache-references                                                        (41.67%)
    18,379,822,612      cache-misses                     #   21.47% of all cache refs           (41.67%)
    54,788,229,200      L1-dcache-load-misses                                                   (41.67%)
        27,107,146      L1-icache-load-misses                                                   (41.67%)
        53,262,327      dTLB-load-misses                                                        (41.67%)
    18,986,130,789      l1_dtlb_misses                                                          (41.67%)
        53,336,284      l2_dtlb_misses                                                          (41.67%)
28,175,235,129,512      instructions                     #    3.87  insn per cycle            
                                                  #    0.00  stalled cycles per insn     (41.67%)
 7,271,772,865,879      cpu-cycles                                                              (41.67%)
   124,407,147,385      stalled-cycles-frontend          #    1.71% frontend cycles idle        (41.67%)
   582,115,368,017      branch-instructions                                                     (41.67%)
     9,979,817,556      branch-misses                    #    1.71% of all branches             (41.67%)
           196,807      page-faults                                                           

    2063.665620797 seconds time elapsed

    2059.293776000 seconds user
       0.484997000 seconds sys


akash@victus:~/Desktop/HPCA$ 

*/