/* Implementare un programma parallelo per l’ambiente
multicore con np unità processanti impieghi la libreria
OpenMP. Il programma deve essere organizzato come
segue: il core master deve leggere una matrice di
dimensione N×N, quindi i core devo collaborare per
ricopiare in parallelo gli elementi della diagonale
principale in un vettore di lunghezza N. Infine, i core
devono effettuare la somma degli elementi di tale
vettore in parallelo. */

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
    int N,sum = 0;
    int **A,*b;

    srand(time(NULL));

    printf("\nDefinisci N: ");
    scanf("%d",&N);

    A = (int**)calloc(N,sizeof(int*));
    for(size_t i = 0; i < N; i++) {
        A[i] = calloc(N,sizeof(int));
    }

    b = (int*)calloc(N,sizeof(int));


    fillMatrix(A,N,N);

    #pragma omp master
    {
        for(size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                printf("%d ",A[i][j]);
            }
            printf("\n");
        }
    }


    #pragma omp parallel for shared(A,b,N)
    for (size_t i = 0; i < N; i++) {
        b[i] = A[i][i];
    }

    #pragma omp parallel for shared(b,N) reduction(+:sum)
    for (size_t i = 0; i < N; i++) {
        sum += b[i];
    }


    printf("\nb: ");
    printVector(b,N);
    printf("\n");

    printf("\nSum: %d\n",sum);
    
    for(size_t i = 0; i < N; i++){
    free(A[i]);
    }
    free(A);
    free(b);

}
