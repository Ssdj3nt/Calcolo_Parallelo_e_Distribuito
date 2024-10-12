#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

void visualizzaArray(int *array, int size) {
    for (int i=0; i<size;i++) {
        printf("%d ",array[i]);
    }
}

int main() {
    int N;

    printf("\nDefinisci N: ");
    scanf("%d",&N);

    int *a = calloc(N,sizeof(int));
    int *b = calloc(N,sizeof(int));
    int *c = calloc(N,sizeof(int));

    for(int i=0; i<N; i++) {
        a[i] = i;
        b[i] = i + 1;
    }

    printf("\nArray A:\n");
    visualizzaArray(a,N);
    printf("\n");
    printf("\nArray B:\n");
    visualizzaArray(b,N);
    printf("\n");

#pragma omp parallel shared (a, b, c)
    {
        int t = omp_get_num_threads();
        int id = omp_get_thread_num();
        int N_Loc = N/t;
        int step;
        int indice;

        int r = N % t; //Calcolo resto della divisione

        if(id < r) //Se il core ha un identificativo minore del resto si deve occupare di calcolare un elemento in più del vettore soluzione C
        {
            N_Loc++;
            step=0;
        } else
        {
            step = r;
        } //Step serve ad ogni core di sapere di quali elementi di A e B deve occuparsi

        for(int i=0; i < N_Loc; i++)
        {
            indice = i + N_Loc * omp_get_thread_num() + step;
            c[indice] = a[indice] + b[indice];
        }
    }

    printf("\nArray c:\n");
    visualizzaArray(c,N);
    printf("\n");

    free(a);
    free(b);
    free(c);

    return 0;
}
