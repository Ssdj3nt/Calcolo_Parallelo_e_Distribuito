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
            printf("%4d",Matrix[i][j]);
        }
        printf("\n");
    }
}


int main(void) {
    int **A,**resultMatrix;
    int num_core = omp_get_num_procs();
    int M = num_core * 2;

    srand(time(NULL));

    printf("\nNumero core: %d\n",num_core);

    allocateMatrix(&A,M,M);
    allocateMatrix(&resultMatrix,M/num_core,M/num_core);

    fillMatrix(A,M,M);

    #pragma omp master
    {
        printf("\nMatrice A:\n\n");
        printMatrix(A,M,M);
    }

    #pragma omp parallel shared(A,M,num_core,resultMatrix)
    {
        int **subMatrix;
        int thread_id = omp_get_thread_num();

        allocateMatrix(&subMatrix,M/num_core,M/num_core);

        printf("\n(Core %d)\n",thread_id);

        int row_start = (thread_id / 4) * 2;
        int col_start = (thread_id % 4) * 2;

        for (int i = row_start; i < row_start + 2; i++) {
            for (int j = col_start; j < col_start + 2; j++) {
                subMatrix[i - row_start][j - col_start] = A[i][j];

                #pragma omp atomic
                resultMatrix[i - row_start][j - col_start] += subMatrix[i - row_start][j - col_start];

            }
        }

        for (size_t i = 0; i < M/num_core; i++) {
            for (size_t j = 0; j < M/num_core; j++) {
                printf("%3d",subMatrix[i][j]);
            }
            printf("\n");
        }

        deallocateMatrix(subMatrix,M/num_core);
    }

    printf("\nMatrice risultato:\n");
    printResMatrix(resultMatrix,M/num_core,M/num_core);

    deallocateMatrix(A,M);
    deallocateMatrix(resultMatrix,M/num_core);

    return 0;
}
