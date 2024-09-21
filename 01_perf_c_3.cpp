/*
Author: akashmaji(@iisc.ac.in)
File  : 01_perf_c_3.cpp
*/

#include<stdio.h>
#include<stdlib.h>

#define ROWS 2048
#define COLS 2048
#define SIZE 2048

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
gcc 01_perf_c_3.cpp -o 01_perf_c_3

Run perf:
perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_c_3

*/

/*------------SAMPLE-------------*/
/*

akash@victus:~/Desktop/HPCA$ perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_c_3
Success: memory allocated!
OK

 Performance counter stats for './01_perf_c_3':

    29,990,592,479 ns   duration_time                                                         
    29,968,686,000 ns   user_time                                                             
        21,001,000 ns   system_time                                                           
     1,520,056,530      cache-references                                                        (41.67%)
       451,815,217      cache-misses                     #   29.72% of all cache refs           (41.67%)
       987,475,910      L1-dcache-load-misses                                                   (41.67%)
           470,355      L1-icache-load-misses                                                   (41.67%)
        11,511,594      dTLB-load-misses                                                        (41.67%)
     9,141,761,248      l1_dtlb_misses                                                          (41.66%)
        11,419,589      l2_dtlb_misses                                                          (41.66%)
   440,773,088,183      instructions                     #    3.79  insn per cycle            
                                                  #    0.00  stalled cycles per insn     (41.66%)
   116,416,101,932      cpu-cycles                                                              (41.66%)
     1,062,864,540      stalled-cycles-frontend          #    0.91% frontend cycles idle        (41.66%)
     9,193,718,720      branch-instructions                                                     (41.66%)
       176,106,034      branch-misses                    #    1.92% of all branches             (41.66%)
            12,381      page-faults                                                           

      29.990592479 seconds time elapsed

      29.968686000 seconds user
       0.021001000 seconds sys


akash@victus:~/Desktop/HPCA$ 

*/
 