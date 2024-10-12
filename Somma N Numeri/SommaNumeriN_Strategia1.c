#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void printVector(int *a, int n);
void insertElem(int *a, int n);

int main(void) {
    int N,sum,sumTot=0,t,N_Loc,rest,id,step;
    srand(time(NULL));

    printf("Definisci il size del vettore N:");
    scanf("%d",&N);

    int *a = calloc(N,sizeof(int));

    insertElem(a,N);
    printVector(a,N);

    #pragma omp parallel private(sum,N_Loc,id,step) shared (sumTot,rest)
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

        sum = 0;

        for(int i=0;i<N_Loc;i++) {
            sum += a[i + N_Loc * id + step];
        }

        sumTot += sum;

    }

    printf("Somma: %d",sumTot);

    free(a);
    return 0;
}

void printVector(int *a , int n) {
    for(int i=0; i<n; i++) {
        printf("%d:%d ",i,a[i]);
    }
    printf("\n");
}
void insertElem(int *a, int n) {
    for(int i=0; i<n; i++) {
        a[i] = rand() % 100;
    }
}
