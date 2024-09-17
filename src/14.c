/*
Implementare un programma parallelo per
l’ambiente multicore con np×np unità processanti che
impieghi la libreria OpenMP. Il programma deve essere
organizzato come segue: il core master deve leggere
una matrice A di dimensione M×M. Quindi, ogni core
deve estrarre un blocco della matrice A di dimensione
(M/np)×(M/np), conservando i valori in np×np matrici.
Infine, i core devono collaborare per calcolare la
somma di tali matrici.
*/

#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

void fillMatrix(int **Matrix, int N, int M) {
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            Matrix[i][j] = rand() % 100;
        }
    }
}

void allocateMatrix(int ***Matrix, int N, int M) {
    *Matrix = (int**)calloc(N, sizeof(int*));
    for (size_t i = 0; i < N; i++) {
        (*Matrix)[i] = (int*)calloc(M, sizeof(int));
    }
}

void deallocateMatrix(int **Matrix, int N) {
    for (size_t i = 0; i < N; i++) {
        free(Matrix[i]);
    }
    free(Matrix);
}

void printMatrix(int **Matrix, int N, int M) {
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            printf("%3d",Matrix[i][j]);
        }
        printf("\n");
    }
}

void printResMatrix(int **Matrix, int N, int M) {
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            printf("%6d",Matrix[i][j]);
        }
        printf("\n");
    }
}


int main(void) {
    int **A,**resultMatrix;
    int num_core = omp_get_num_procs();
    int M = num_core * 2;
    int blockSize = M / num_core;

    srand(time(NULL));

    printf("\nNumero core: %d\n",num_core);

    allocateMatrix(&A,M,M);
    allocateMatrix(&resultMatrix,blockSize,blockSize);

    fillMatrix(A,M,M);

    #pragma omp master
    {
        printf("\nMatrice A:\n\n");
        printMatrix(A,M,M);
    }

    omp_set_num_threads(num_core * num_core);

    #pragma omp parallel shared(A,M,resultMatrix,blockSize)
    {
        int **subMatrix;
        int thread_id = omp_get_thread_num();

        allocateMatrix(&subMatrix,blockSize,blockSize);

        printf("\n(Core %d)\n",thread_id);

        int num_blocks_per_row = M / blockSize;

        int blockRow = thread_id / num_blocks_per_row;
        int blockCol = thread_id % num_blocks_per_row;
        int row_start = blockRow * blockSize;
        int col_start = blockCol * blockSize;

        for (int i = row_start; i < row_start + blockSize; i++) {
            for (int j = col_start; j < col_start + blockSize; j++) {
                subMatrix[i - row_start][j - col_start] = A[i][j];

                #pragma omp atomic
                resultMatrix[i - row_start][j - col_start] += subMatrix[i - row_start][j - col_start];

            }
        }

        for (size_t i = 0; i < blockSize; i++) {
            for (size_t j = 0; j < blockSize; j++) {
                printf("%3d",subMatrix[i][j]);
            }
            printf("\n");
        }

        deallocateMatrix(subMatrix,blockSize);
    }

    printf("\nMatrice risultato:\n");
    printResMatrix(resultMatrix,blockSize,blockSize);

    deallocateMatrix(A,M);
    deallocateMatrix(resultMatrix,blockSize);

    return 0;
}
