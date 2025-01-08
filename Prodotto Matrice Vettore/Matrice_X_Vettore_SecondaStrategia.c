#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void fillMatrix(int **Matrix, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            Matrix[i][j] = i + j;
        }
    }
}

void fillVector(int *Vector, int m) {
    for (int i = 0; i < m; i++) {
        Vector[i] = i;
    }
}

void printMatrix(int **Matrix, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
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
    int n, m;

    printf("\nDefinisci le righe della matrice: ");
    scanf("%d", &n);
    printf("\nDefinisci le colonne della matrice: ");
    scanf("%d", &m);

    // Alloco la matrice
    int **Matrix = calloc(n, sizeof(int*));
    for (int i = 0; i < n; i++) {
        Matrix[i] = (int*)calloc(m, sizeof(int));
    }

    // Alloco i vettori
    int *Vector = calloc(m, sizeof(int));
    int *Result = calloc(m, sizeof(int));

    fillMatrix(Matrix, n, m);
    fillVector(Vector, m);

    printf("\nMatrice:\n");
    printMatrix(Matrix, n, m);

    printf("\nVettore:\n");
    printVector(Vector, m);

    // Calcolo del prodotto matrice-vettore in parallelo
#pragma omp parallel for shared(Matrix, Vector) reduction(+:Result[:m])
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++) {
            Result[i] += Matrix[i][j] * Vector[j];
        }
    }

    printf("\nRisultato del prodotto matrice-vettore:\n");
    printVector(Result, m);
    printf("\n");

    // Liberazione della memoria
    for (int i = 0; i < n; i++) {
        free(Matrix[i]);
    }
    free(Matrix);
    free(Vector);
    free(Result);

    return 0;
}
