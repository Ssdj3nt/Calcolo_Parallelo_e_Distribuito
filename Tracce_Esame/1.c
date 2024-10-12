/*  Implementazione di un algoritmo parallelo (np core)
per il calcolo dell’operazione c=a+b con a,b vettori di
dimensione N, in ambiente openMP */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void fillVector(int *vector, int size) {
    for (size_t i = 0; i < size; i++) {
        vector[i] = rand() % 100 + 1;
    }
}

void printVector(int *vector, int size) {
    for (size_t i = 0; i < size; i++) {
        printf("%d ",vector[i]);
    }
}

int main(void) {
    int N;
    int *a,*b,*c;

    srand(time(NULL));

    printf("\nDefinisci N: ");
    scanf("%d",&N);

    a = (int*)calloc(N,sizeof(int));
    b = (int*)calloc(N,sizeof(int));
    c = (int*)calloc(N,sizeof(int));

    fillVector(a,N);
    fillVector(b,N);

    #pragma omp parallel for shared(a,b,c,N)
    for (size_t i = 0; i < N; i++) {
        c[i] = a[i]+b[i];
    }

    printf("\nA: ");
    printVector(a,N);
    printf("\n");

    printf("\nB: ");
    printVector(b,N);
    printf("\n");


    printf("\nC: ");
    printVector(c,N);
    printf("\n");

    free(a);
    free(b);
    free(c);

}

