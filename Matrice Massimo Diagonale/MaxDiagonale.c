/*
Assegnata una matrice di dimensione N×N, estrarre in parallelo gli elementi
della diagonale principale e ricopiarli in un vettore, quindi calcolarne il
massimo, con una procedura parallela
*/

#include <stdio.h>
#include <stdlib.h>

void fillMaxtrix(int **Matrix, int n, int m) {
    for(size_t i = 0; i < n; i++)
        for(size_t j = 0; j < m; j++) {
            Matrix[i][j] = i+j;
        }
}

void printMaxtrix(int **Matrix, int n, int m) {
    printf("\nMatrice:\n");
    for(size_t i = 0; i < n; i++) {
        for(size_t j = 0; j < m; j++) {
            printf("%d ",Matrix[i][j]);
        }
        printf("\n");
    }
}

void printVector(int *Vector, int n) {
    printf("\nVector:\n");
    for (size_t i = 0; i < n; i++) {
        printf("%d ",Vector[i]);
    }
}

int main(void) {
    int n,m;


    printf("\nDefinisci il size delle righe della matrice:");
    scanf("%d",&n);
    printf("\nDefinisci il size delle colonne della matrice:");
    scanf("%d",&m);

    int min_dim = (n < m) ? n : m;

    int **Matrix = calloc(n,sizeof(int *));
    for(size_t i = 0; i < n; i++) {
        Matrix[i] = (int *)calloc(m,sizeof(int));
    }

    int *Vector = calloc(n,sizeof(int));

    fillMaxtrix(Matrix,n,m);
    printMaxtrix(Matrix,n,m);

    #pragma omp parallel for
    for (size_t i = 0; i < min_dim; i++) {
        Vector[i] = Matrix[i][i];
    }

    #pragma omp barrier

    int MaxDiag = Vector[0];

    #pragma omp parallel for reduction(max:MaxDiag)
    for(size_t i = 0; i < n; i++) {
        MaxDiag = (MaxDiag < Vector[i]) ? Vector[i] : MaxDiag;
    }

    printVector(Vector,n);
    printf("\n\nMax diag: %d\n",MaxDiag);


    for(size_t i; i < n; i++) {
        free(Matrix[i]);
    }
    free(Matrix);
    free(Vector);
    return 0;

}
