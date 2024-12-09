//il core master deve generare una matrice A di NXM
//ogni core deve estrarre un bocco riga dalla matrice generata e conservarlo in una sottomatrice locale A_LOC_I, i=0,...,np-1;
//i core devono collabborare per effettuare, in parallelo, il prodotto puntuale tra le sottomatrici locali A_loc_i ottenute.
//infine il core master deve stampare il risultato finale ed tempo di esecuzione

#include "stdio.h"
#include "stdlib.h"
#include "omp.h"
#include "time.h"

void allocateMatrix(int ***matrix, size_t n, size_t m) {
    *matrix = (int**)calloc(n, sizeof(int*));
    for (size_t i = 0; i < n; i++) {
        (*matrix)[i] = (int*)calloc(m, sizeof(int));
    }
}

void deallocateMatrix(int **matrix, size_t n) {
    for (size_t i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void fillMatrix(int **Matrix, size_t n, size_t m) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            Matrix[i][j] = rand() % 9;
        }
    }
}

void printMatrix(int **Matrix, size_t n, size_t m) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            printf("%d ", Matrix[i][j]);
        }
        printf("\n");
    }
}

void allocateVector(unsigned long **vector, size_t m) {
    *vector = (unsigned long*)calloc(m, sizeof(unsigned long));
}

void deallocateVector(unsigned long *vector) {
    free(vector);
}

void printVector(unsigned long *vector, size_t m) {
    for (size_t i = 0; i < m; i++) {
        printf("%lu ", vector[i]);
    }
    printf("\n");
}

void fillVector(unsigned long *vector, size_t m) {
    for (size_t i = 0; i < m; i++) {
        vector[i] = 1;
    }
}

int main() {
    srand(time(NULL));
    int **matrix;
    unsigned long *resVector , res = 0;
    int N, M, np;
    double start_time = 0, end_time = 0;

    printf("\nDefinisci le righe della matrice:\n");
    scanf("%d", &N);
    printf("\nDefinisci le colonne della matrice:\n");
    scanf("%d", &M);
    printf("\nDefinisci il numero di core da utilizzare:\n");
    scanf("%d", &np);
    printf("\n");

    int t = N / np;
    int remainder = N % np;
    int id, startRow, endRow;
    size_t i, j, k;
    
    #pragma omp master
    {
        allocateMatrix(&matrix, N, M);
        fillMatrix(matrix, N, M);
        printf("\nMatrice\n");
        printMatrix(matrix, N, M);
    }
    
    allocateVector(&resVector, M);
    fillVector(resVector, M);

    start_time = omp_get_wtime();

#pragma omp parallel num_threads(np) shared(matrix, N, M) private(id, startRow, endRow, i, j, k) firstprivate(t, remainder) reduction(*:resVector[:M])
{
    unsigned long *A_loc_i;
    allocateVector(&A_loc_i, M);
    
    id = omp_get_thread_num();
    startRow = id * t + (id < remainder ? id : remainder);
    endRow = startRow + t + (id < remainder ? 1 : 0);

    for (i = startRow; i < endRow; i++) {
        for (j = 0; j < M; j++) {
            A_loc_i[j] = matrix[i][j];
        }
      
        for (k = 0; k < M; k++) {
            resVector[k] *= A_loc_i[k];
        }
        
        #pragma omp critical
        {
         printf("\nCore %d\n",id);
         printVector(A_loc_i,M);
         printf("\n");
        }
    }
  deallocateVector(A_loc_i);
}

    #pragma omp parallel for num_threads(np) private(i) reduction(+:res)
    for (i = 0; i < M; i++) {
        res += resVector[i];
    }

    end_time = omp_get_wtime();

    #pragma omp master
    {
        printf("\nRESVECTOR:\n");
        printVector(resVector, M);
        printf("\nRisultato finale: %d\n", res);
        printf("\nTempo impiegato: %lf secondi\n\n", end_time - start_time);
    }

    deallocateMatrix(matrix, N);
    deallocateVector(resVector);
}

