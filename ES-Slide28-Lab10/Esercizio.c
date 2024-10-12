/*
* Implementare un programma parallelo per l'ambiente multicore con np
* unità processanti che impieghi la libreria OpenMP. Il programma deve
* essere organizzato come segue:
*
* 1. il core master deve leggere una matrice A di dimensione NxM
*
* 2. i core devono organizzarsi per estrarre ognuno una sottomatrice,
*    decomponendo la matrice A per blocchi riga, e costruendo la trasposta
*    di tale sottomatrice, in una matrice Bloc
*
* 3. infine i core devono collaborare per sommare le sotto matrici estratte Bloc
*    in un'unica matrice finale C
*
* 4. il core master stampa la matrice risutato e il tempo d'esecuzione
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void allocateMatrix(int ***Matrix, int N, int M) {
    *Matrix = (int**)calloc(N,sizeof(int*));
    for (int i = 0; i < N; i++) {
        (*Matrix)[i] = (int*)calloc(M,sizeof(int));
    }
}

void deallocateMatrix(int **Matrix, int N) {
    for (int i = 0; i < N; i++) {
        free(Matrix[i]);
    }
    free(Matrix);
}

void fillMatrix(int **Matrix, int n, int m) {
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++) {
            Matrix[i][j] = j;
        }
}

void printMatrix(int **matrix, int n, int m) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            printf("%d ",matrix[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    int N,M;
    int **A,**Bloc;

    printf("\nDefinisci N:");
    scanf("%d",&N);
    printf("\nDefinisci M:");
    scanf("%d",&M);

    allocateMatrix(&A,N,M);
    allocateMatrix(&Bloc,M,N);
    int *C = calloc(N,sizeof(int));
    fillMatrix(A,N,M);

    double start = omp_get_wtime();
    #pragma omp master
    {
        printf("\nMatrice A:\n");
        printMatrix(A,N,M);
    }

    #pragma barrier

    #pragma omp parallel for shared(A,Bloc) schedule(static) reduction(+:C[:N])
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            Bloc[j][i] = A[i][j];
            C[i] += Bloc[j][i];
        }
    }
    double end = omp_get_wtime();


    printf("\nMatrice Bloc:\n");
    printMatrix(Bloc,M,N);

    #pragma omp master
    {
        printf("\nMatrice C:\n");
        for (int i = 0; i < N; i++) {
            printf("%d ",C[i]);
        }
        printf("\n \nTime:%lf",end-start);
    }


    deallocateMatrix(A,N);
    deallocateMatrix(Bloc,M);
    free(C);

}
