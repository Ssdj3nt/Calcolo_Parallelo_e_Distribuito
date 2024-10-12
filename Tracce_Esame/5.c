/* 
Implementare un programma parallelo per l’ambiente
multicore con np unità processanti che impieghi la
libreria OpenMP. Il programma deve essere
organizzato come segue: il core master deve generare una matrice di dim N×N, ogni core deve
estrarre N/np righe e calcolare il prodotto puntuale tra
i vettori corrispondenti alle righe estratte.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void fillMatrix(int **Matrix, int row, int column) {
    for(size_t i = 0; i < row; i++)
        for (size_t j = 0; j < column; j++) {
            Matrix[i][j] = rand() % 50 + 1;
        }
}

void printVector(int *vector, int size) {
    for (size_t i = 0; i < size; i++) {
        printf("%d ",vector[i]);
    }
}

int main(void) {
    int N;
    int **A,*res;

    srand(time(NULL));

    printf("\nDefinisci N: ");
    scanf("%d",&N);

    res = (int*)calloc(N,sizeof(int));
    for(size_t i = 0; i < N; i++) {
        res[i] = 1;
    }
    printf("\nRes:\n");
    printVector(res,N);

#pragma omp master
    {
        A = (int**)calloc(N,sizeof(int*));
        for(size_t i = 0; i < N; i++) {
            A[i] = calloc(N,sizeof(int));
        }
        fillMatrix(A,N,N);

        printf("\n\nMatrix A:\n");
        for(size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                printf("%d ",A[i][j]);
            }
            printf("\n");
        }
    }

#pragma omp parallel for shared(A,N)
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {

#pragma omp critical
            {
                res[j] *= A[i][j];
            }
        }
    }

    printf("\nRes:\n");
    printVector(res,N);
    printf("\n");

    for(size_t i = 0; i < N; i++){
        free(A[i]);
    }
    free(A);
    free(res);
}
