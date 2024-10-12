#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

void visualizzaArray(int *array, int size) {
    for (int i=0; i<size;i++) {
        printf("%d ",array[i]);
    }
}

int main() {
    int num_procs = omp_get_num_procs();
    int N;

    do {
        printf("\nDefinisci N: ");
        scanf("%d",&N);
        if(N % num_procs != 0) {
            printf("La divisione tra il numero dei sottoproblemi ed il numero dei core deve ritornare un intero.\n");
        }
    } while (N % num_procs != 0);

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

    #pragma omp parallel shared (a,b,c)
    {
        int indice;
        int t = omp_get_num_threads();
        int N_Loc = N/t;

        for(int i=0;i<N_Loc;i++) {
            indice = i + N_Loc * omp_get_thread_num();
            c[indice] = a[indice] + b[indice];
        }
    }

    printf("\nArray c:\n");
    visualizzaArray(c,N);

    free(a);
    free(b);
    free(c);
    
    return 0;
}
