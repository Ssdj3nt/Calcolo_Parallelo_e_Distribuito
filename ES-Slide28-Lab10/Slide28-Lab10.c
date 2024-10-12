/*
Slide 28 Laboratorio 10
Implementare un programma parallelo per l’ambiente multicore con npunità processanti che impieghi la libreria OpenMP. Il programma deve
essere organizzato come segue:

1)il core master deve generare una matrice B di dimensione NxN e due
vettori a, b di lunghezza N

2)i core devono collaborare per costruire, in parallelo, una nuova matrice A
ottenuta sommando alla diagonale principale della matrice B il vettore b

3)quindi, i core devono collaborare per calcolare in parallelo il prodotto tra
la nuova matrice A ed il vettore a, distribuendo il lavoro per colonne


4)infine, il core master stampa il risultato finale ed il tempo d’esecuzione
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void fillMatrixA(int **A, int n) {
    for (size_t i = 0; i < n; i++)
        for (size_t j = 0; j < n; j++) {
            A[i][j] = 1;
        }
}

void fillMatrixB(int **B, int n) {
    for (size_t i = 0; i < n; i++)
        for (size_t j = 0; j < n; j++) {
            B[i][j] = 3;
        }
}

void fillVectorA(int *a, int n) {
    for (size_t i = 0; i < n; i++) {
        a[i] = 3;
    }
}

void fillVectorB(int *b, int n) {
    for (size_t i = 0; i < n; i++) {
        b[i] = 4;
    }
}

void printMatrix(int **Matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", Matrix[i][j]);
        }
        printf("\n");
    }
}

void printVector(int *Vector, int x) {
    for (int i = 0; i < x; i++) {
        printf("%d ", Vector[i]);
    }
    printf("\n");
}

int main(void) {
    double start_time = omp_get_wtime(), end_time;
    int N;
    int **A,**B;
    int *a,*b;
    int *res;

    srand(time(NULL));

    #pragma omp master
    {
        N = rand() % 5 + 1;
        B = (int**)calloc(N,sizeof(int*));
        for(size_t i = 0; i < N; i++) {
            B[i] = calloc(N,sizeof(int));
        }
        a = (int*)calloc(N,sizeof(int));
        b = (int*)calloc(N,sizeof(int));
    }

    //Definisco le colonne della matrice.
    #pragma omp single
    {
        A = (int**)calloc(N,sizeof(int *));
        res = (int *)calloc(N,sizeof(int));
    }

    //Parallelizzo l'allocazione delle righe della matrice.

    #pragma omp parallel for
    for(size_t i = 0; i < N; i++) {
        A[i] = calloc(N,sizeof(int));
    }

    #pragma omp barrier

    fillMatrixA(A,N);
    fillMatrixB(B,N);
    fillVectorA(a,N);
    fillVectorB(b,N);

    #pragma omp barrier

    #pragma omp parallel for
    for (size_t i = 0; i < N; i++) {
        A[i][i] = B[i][i] + b[i];
    }

    #pragma omp barrier

    #pragma omp parallel for reduction(+:res[:N])
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) {
            res[i] += A[i][j] * a[j];
        }
    }

    printf("\nMatrice A:\n");
    printMatrix(A,N);
    printf("\nMatrice B:\n");
    printMatrix(B,N);
    printf("\nVettore a: ");
    printVector(a,N);
    printf("\nVettore b: ");
    printVector(b,N);

    end_time = omp_get_wtime();
    
    #pragma omp master
    {
        printf("\nVettore Risultato: ");
        printVector(res,N);
        printf("\nTempo di esecuzione: %lf\n",end_time-start_time);
    }

    for (size_t i = 0; i < N; i++) {
        free(A[i]);
    }
    free(A);
    for (size_t i = 0; i < N; i++) {
        free(B[i]);
    }
    free(B);
    free(a);
    free(b);
    free(res);
}
