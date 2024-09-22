/*
Author : akashmaji(@iisc.ac.in)
File   : 01_perf_b_1b.cpp
*/
#include<stdio.h>
#include<stdlib.h>
#include<linux/mman.h>
#include<sys/mman.h>

#define ROWS 8192
#define COLS 8192
#define SIZE 8192

#define LIMIT 1000

#define MB_2 (2*1024*1024)

/*
Some Calculation on large pages (2MB)
matrix size : 8192 * 8192 elements
            : 2^26 ints
            : 2^28 B
            : 256 MB
            : 128 huge pages per matrix
total size  : 3 matrices A, B, C
            : 3 * 128 huge pages each
            : 384 huge pages
That means, we must have 384 huge pages each of size 2MB allocated
*/

#define NMAP_ALLOC_SIZE (MB_2 * 128)

void print_matrix(int* matrix, int N, int M){
    for(int row = 0; row < N; row++){
        for(int col = 0; col < M; col++){
            printf("%d ", matrix[row * N + col]);
        }
        printf("\n");
    }
}


int main(){
    
    // printf("Allocating...\n");
    // allocate memory dynamically to matrices A, B and C using nmap with appropriate size and flags
    int *matrixA = (int*)mmap(NULL,
                      NMAP_ALLOC_SIZE,
                      PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB | MAP_HUGE_2MB,
                      -1, 0);
    int* matrixB = (int*)mmap(NULL,
                      NMAP_ALLOC_SIZE,
                      PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB | MAP_HUGE_2MB,
                      -1, 0);
    int* matrixC = (int*)mmap(NULL,
                      NMAP_ALLOC_SIZE,
                      PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB | MAP_HUGE_2MB,
                      -1, 0);

    // sanity check that nmap() has indeed allocated huge pages
    if(matrixA == NULL || matrixB == NULL || matrixC == NULL){
        printf("Error: nmap() failed to allocate memory!");
        exit(0);
    }

    // printf("Initializing...\n");
    // initialize matrices A and B with some random values within 'LIMIT' and C with zeros
    for(int row = 0; row < ROWS; row++){
        for(int col = 0; col < COLS; col++){
            matrixA[row * COLS + col] = rand() % LIMIT;
            matrixB[row * COLS + col] = rand() % LIMIT;
            matrixC[row * COLS + col] = 0;
        }
    }

    // char ch;
    // printf("Success: nmap() allocated. Enter a char to continue:");
    // scanf("%c", &ch);

    // do the matrix multiplication C = A * B
    // loop order is (i, j, k)
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            for(int k = 0; k < SIZE; k++){
                matrixC[i * COLS + j] += (matrixA[i * COLS + k] * matrixB[k * COLS + j]);
            }
        }
        // printf("Done one row: %d\n", i);
    }
   
    // see matrix C
    // print_matrix(matrixC, ROWS, COLS);


    // printf("Deallocating...\n");
    // unallocate memeory allocated by nmap() using munmap()
    munmap(matrixA, NMAP_ALLOC_SIZE);
    munmap(matrixB, NMAP_ALLOC_SIZE);
    munmap(matrixC, NMAP_ALLOC_SIZE);


    return 0;
}

/*--------Instructions-------------*/
/*

Compile:
gcc 01_perf_b_1b.cpp -o 01_perf_b_1b

Run perf:
perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_b_1b

*/

/*------------SAMPLE-------------*/
/*

akash@victus:~/Desktop/HPCA$ perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_b_1b


 Performance counter stats for './01_perf_b_1b':

 5,430,822,197,871 ns   duration_time                                                         
 5,426,222,405,000 ns   user_time                                                             
       299,995,000 ns   system_time                                                           
 2,208,734,405,104      cache-references                                                        (41.67%)
 1,215,439,595,283      cache-misses                     #   55.03% of all cache refs           (41.67%)
   987,842,568,359      L1-dcache-load-misses                                                   (41.67%)
       218,222,364      L1-icache-load-misses                                                   (41.67%)
        17,960,598      dTLB-load-misses                                                        (41.67%)
     8,950,318,669      l1_dtlb_misses                                                          (41.67%)
        18,100,999      l2_dtlb_misses                                                          (41.67%)
24,788,744,593,334      instructions                     #    1.17  insn per cycle            
                                                  #    0.02  stalled cycles per insn     (41.67%)
21,121,286,004,178      cpu-cycles                                                              (41.67%)
   607,597,544,697      stalled-cycles-frontend          #    2.88% frontend cycles idle        (41.67%)
   562,573,540,888      branch-instructions                                                     (41.67%)
     1,501,908,481      branch-misses                    #    0.27% of all branches             (41.67%)
               438      page-faults                                                           

    5430.822197871 seconds time elapsed

    5426.222405000 seconds user
       0.299995000 seconds sys


akash@victus:~/Desktop/HPCA$ 

*/
