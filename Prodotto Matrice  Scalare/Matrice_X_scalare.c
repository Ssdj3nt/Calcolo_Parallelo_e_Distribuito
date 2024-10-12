#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

void visualizzaMatrice(int **matrix, int n, int m) {
    printf("\nMatrice:\n");
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            printf("%d ",matrix[i][j]);
        }
        printf("\n");
    }
}
void fillMatrix(int **matrix, int n, int m) {
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++) {
            matrix[i][j] = 1;
        }
}
void fillMatrixRandom(int **matrix, int n, int m) {
    srand(time(NULL));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            matrix[i][j] = rand() % 100 + 1;
        }
    }
}

int main(void) {
    int n,m,scalar,choice;

    printf("\nDefinisci il size delle righe della matrice:");
    scanf("%d",&n);
    printf("\nDefinisci il size delle colonne della matrice:");
    scanf("%d",&m);

    //Allocazione della matrice come array di puntatori
    int **matrix = calloc(n,sizeof (int *));
    for(int i = 0; i < n; i++) {
        matrix[i] = (int*)calloc(m,sizeof(int));
    }

    printf("\nDefinisci lo scalare:");
    scanf("%d",&scalar);

    printf("\n[1] Riempi la matrice con tutti 1.");
    printf("\n[2] Riempi la matrice in modo pseudo-casuale.");
    printf("\nScelta:");

    scanf("%d",&choice);
    if(choice==1) {
        fillMatrix(matrix,n,m);
    }
    else
        fillMatrixRandom(matrix,n,m);

    printf("\nInput");
    visualizzaMatrice(matrix,n,m);

#pragma omp parallel for shared(matrix,n,m,scalar)
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            matrix[i][j] = matrix[i][j] * scalar;
        }
    }

    printf("\nOutput");
    visualizzaMatrice(matrix,n,m);

    for(int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
    return 0;

}
