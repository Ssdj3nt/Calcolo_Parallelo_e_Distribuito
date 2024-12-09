//il core master deve generare una matrice A di NXM
//ogni core deve estrarre un bocco colonna dalla matrice generata e conservarlo in una sottomatrice locale A_LOC_I, i=0,...,np-1;
//i core devono collabborare per effettuare, in parallelo, il prodotto puntuale tra le sottomatrici locali A_loc_i ottenute.
//infine il core master deve stampare il risultato finale ed tempo di esecuzione

#include "stdio.h"
#include "omp.h"
#include "stdlib.h"
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
    int **matrix, N, M, np;
    unsigned long *vector, res = 0;
    double start_time = 0, end_time = 0;

    printf("\nN:\n");
    scanf("%d", &N);
    printf("\nM:\n");
    scanf("%d", &M);
    printf("\nNp:\n");
    scanf("%d", &np);
    
    allocateMatrix(&matrix, N, M);
    allocateVector(&vector, N);
    fillMatrix(matrix, N, M);
    fillVector(vector,N);
    
    printf("\nMatrice:\n");
    printMatrix(matrix, N, M);

    start_time = omp_get_wtime();
    
    #pragma omp parallel num_threads(np) reduction(*:vector[:N])
    {
        int id = omp_get_thread_num();
        unsigned long *A_loc_i;
        allocateVector(&A_loc_i, N);
        
        int startCol = id * (M / np) + (id < M % np ? id : M % np);
        int endCol = startCol + (M / np) + (id < M % np ? 1 : 0) - 1;
        
        for (size_t j = startCol; j <= endCol; j++) {
            for (size_t i = 0; i < N; i++) {
                A_loc_i[i] = matrix[i][j];
            }

            #pragma omp critical
            {
                printf("\nCore:%d, Colonna %d:\n", id, j);
                printVector(A_loc_i, N);
                printf("\n");
            }
            
        for(size_t k = 0; k < N; k++) {
         vector[k] *= A_loc_i[k];
         }
         
        }
        deallocateVector(A_loc_i);
    }
    
    #pragma omp parallel for num_threads(np) reduction(+:res)
    for(size_t i = 0; i < N; i++){
     res += vector[i];
    }
    
    end_time = omp_get_wtime();
    
    #pragma omp master
    {
        printf("\nRESVECTOR:\n");
        printVector(vector, N);
        printf("\nRisultato finale: %lu\n", res);
        printf("\nTempo impiegato: %lf secondi\n\n", end_time - start_time);
    }
    
    deallocateVector(vector);
    deallocateMatrix(matrix, N);
}

