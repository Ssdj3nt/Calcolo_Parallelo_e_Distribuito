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
    int machine_num_core = omp_get_num_procs(); // Numero di core della macchina

    srand(time(NULL));

    // Imposta la dimensione della matrice in base al numero di core
    N = machine_num_core * 2;
    int blockSize = N / machine_num_core; // Dimensione del blocco

    // Alloca e inizializza l'array per i risultati
    b = (int*)calloc(machine_num_core * machine_num_core, sizeof(int));

    // Alloca e inizializza la matrice A
    A = (int**)calloc(N, sizeof(int*));
    for (size_t i = 0; i < N; i++) {
        A[i] = (int *)calloc(N, sizeof(int));
    }

    // Riempie la matrice A con valori casuali e la stampa
    printf("\nMatrice A (%dx%d):\n\n", N, N);
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            A[i][j] = rand() % 100 + 1;
            printf("%2d ", A[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // Imposta il numero di thread
    omp_set_num_threads(machine_num_core * machine_num_core);

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int num_blocks_per_row = N / blockSize;

        // Calcola l'inizio della riga e della colonna per il blocco
        int blockRow = thread_id / num_blocks_per_row;
        int blockCol = thread_id % num_blocks_per_row;
        int row_start = blockRow * blockSize;
        int col_start = blockCol * blockSize;

        // Calcola la somma degli elementi nel blocco
        int local_sum = 0;
        for (int i = row_start; i < row_start + blockSize; i++) {
            for (int j = col_start; j < col_start + blockSize; j++) {
                local_sum += A[i][j];
            }
        }

        // Memorizza la somma nel vettore b
        #pragma omp critical
        {
            b[thread_id] = local_sum;
        }
    }

    // Stampa i risultati
    printf("Array b (somme dei blocchi):\n");
    for (size_t i = 0; i < machine_num_core * machine_num_core; i++) {
        printf("Blocco %zu: %d\n", i, b[i]);
    }

    // Libera la memoria
    for(size_t i = 0; i < N; i++) {
        free(A[i]);
    }
    free(A);
    free(b);

    return 0;
}
