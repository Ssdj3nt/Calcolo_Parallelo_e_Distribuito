/* Implementazione di un algoritmo parallelo (np core)
per il calcolo degli elementi di un vettore c, i cui valori
sono ottenuti moltiplicando l’identificativo del core per
gli elementi di un vettore a di dimensione N, in
ambiente openMP */



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
    int *a,*c;

    srand(time(NULL));

    printf("\nDefinisci N: ");
    scanf("%d",&N);

    a = (int*)calloc(N,sizeof(int));
    c = (int*)calloc(N,sizeof(int));

    fillVector(a,N);

    #pragma omp parallel for shared(a,c,N)
    for (size_t i = 0; i < N; i++) {
        c[i] = omp_get_thread_num() * a[i];;
        printf("\nSono il core: %d\n",omp_get_thread_num());
    }

    printf("\nA: ");
    printVector(a,N);
    printf("\n");



    printf("\nC: ");
    printVector(c,N);
    printf("\n");

    free(a);
    free(c);

}
