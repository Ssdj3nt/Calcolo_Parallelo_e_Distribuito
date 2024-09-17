/*  
Implementare un programma parallelo per
l’ambiente multicore con np×np unità processanti che
impieghi la libreria OpenMP. Il programma deve essere
organizzato come segue: il core master deve generare
una matrice A di dimensione N×N. Quindi, ogni core
deve sommare tra loro gli elementi di un blocco
quadrato della matrice A di dimensione (N/np)×(N/np),
conservando i valori in un vettore b.
*/

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    int N;
    int **A;
    int *b;
    int machine_num_core = omp_get_num_procs();

    srand(time(NULL));

    printf("\nNumero di core della macchina: %d\n",machine_num_core);
    printf("\nAlloco una matrice NxN con N = 2xNP, quindi %dx%d.\n\n",machine_num_core*2,machine_num_core*2);

    N = machine_num_core * 2;

    b = (int*)calloc(machine_num_core,sizeof(int));

    #pragma omp master
    {
        A = (int**)calloc(N,sizeof(int*));
        for (size_t i = 0; i < N; i++) {
            A[i] = (int *)calloc(N,sizeof(int));
        }

        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                A[i][j] = rand() % 100 + 1;
                printf("%3d ",A[i][j]);
            }
            printf("\n");
        }
    }

    printf("\n");

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();

        // Determinare il blocco per ogni core
        int row_start = (thread_id / 4) * 2;
        int col_start = (thread_id % 4) * 2;

        for (int i = row_start; i < row_start + 2; i++) {
            for (int j = col_start; j < col_start + 2; j++) {
                b[thread_id] += A[i][j];
            }
        }
    }

    printf("Array b: ");
    for (size_t i = 0; i < machine_num_core; i++) {
        printf("%d ",b[i]);
    }
    printf("\n");

    for(size_t i = 0; i < N; i++) {
        free(A[i]);
    }
    free(A);
    free(b);
}

