#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void printVector(int *a, int n);
void insertElem(int *a, int n);

int main(void) {
    int N, t, N_Loc, rest, id, step;
    unsigned long sum = 0;

    srand(time(NULL));

    printf("Definisci il size del vettore N:");
    scanf("%d",&N);

    int *a = calloc(N,sizeof(int));
    if(a == NULL) {
        printf("Errore allocazione memoria.");
        exit(1);
    }

    insertElem(a,N);
    printVector(a,N);

    #pragma omp parallel shared(a,N) private (N_Loc,rest,id,step) reduction(+:sum)
    {
        t = omp_get_num_threads();
        id = omp_get_thread_num();
        N_Loc = N/t;
        rest = N%t;

        if(id < rest) {
            N_Loc++;
            step = 0;
        }
        else
            step = rest;

        for(size_t i = 0; i < N_Loc; i++) {
            size_t index = i + N_Loc * id + step;
            sum += a[index];
        }

    }

    printf("Somma: %lu",sum);

    free(a);
    return 0;
}

void printVector(int *a , int n) {
    for(size_t i=0; i<n; i++) {
        printf("%d:%d ",i,a[i]);
    }
    printf("\n");
}
void insertElem(int *a, int n) {
    for(size_t i=0; i<n; i++) {
        a[i] = rand() % 100;
    }
}
