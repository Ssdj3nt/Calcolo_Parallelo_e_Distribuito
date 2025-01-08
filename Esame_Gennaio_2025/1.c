#include "stdio.h"
#include "stdlib.h"
#include "omp.h"
#include "time.h"

// Alloca un vettore.
void allocateVector(int **vector, size_t n) {
 *vector = (int*)calloc(n,sizeof(int));
}

// Dealloca un vettore.
void deallocateVector(int *vector) {
    free(vector);
}

void fillVector(int *Vector, size_t n) {
    for (size_t i = 0; i < n; i++) {
        Vector[i] = rand() % 10;
    }
}

// Stampa il vettore.
void printVector(int *Vector, size_t n) {
    for (size_t i = 0; i < n; i++) {
        printf("%d ", Vector[i]);
    }
    printf("\n");
}

// Alloca una matrice.
void allocateMatrix(int ***matrix, size_t n, size_t m){
 *matrix = (int**)calloc(n, sizeof(int*));
 for (size_t i = 0; i < n; i++) {
    (*matrix)[i] = (int*)calloc(m, sizeof(int));
    }
}

// Dealloca una matrice.
void deallocateMatrix(int **matrix, size_t n) {
    for (size_t i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Riempi una matrice in modo randomico.
void fillMatrix(int **Matrix, size_t n, size_t m) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            Matrix[i][j] = rand() % 10;
        }
    }
}

// Stampa la matrice.
void printMatrix(int **Matrix, size_t n, size_t m) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            printf("%d ", Matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
   int *Vettore, *Risultato, **Matrice, np;
   size_t N,M;
   double start_time = 0, end_time = 0;
   
   srand(time(NULL));
   
   printf("\nDefinisci il numero di righe:\n");
   scanf("%zu",&N);
   
   printf("\nDefinisci il numero di colonne:\n");
   scanf("%zu",&M);
   
   printf("\nDefinisci il numero di core:\n");
   scanf("%d",&np);
   
   allocateVector(&Risultato, M);
   
   // Generazione Vettore "b" e Matrice "A".
   #pragma omp master
   {
      allocateVector(&Vettore, M);
      allocateMatrix(&Matrice, N, M);
      fillVector(Vettore, M);
      fillMatrix(Matrice, N, M);
   }
   
   printf("\nVettore:\n");
   printVector(Vettore, M);

   printf("\nMatrice\n");
   printMatrix(Matrice, N, M);
   
   start_time = omp_get_wtime();
       
   // Calcolo del prodotto matrice-vettore in parallelo, 2a strategia.
   #pragma omp parallel for shared(Matrice, Vettore) num_threads(np) reduction(+:Risultato[:M])
      for (size_t j = 0; j < M; j++) {
         for (size_t i = 0; i < N; i++) {
            Risultato[i] += Matrice[i][j] * Vettore[j];
         }
      }
  
  // Prodotto tra id-core ed elemento i-esimo del vettore.
  #pragma omp parallel for shared(Risultato) num_threads(np)
  for(size_t i = 0; i < M; i++) {
     Risultato[i] = Risultato[i] * omp_get_thread_num();
  }
  
  end_time = omp_get_wtime();
  
  // Stampa del vettore risultante e del tempo di esecuzione.
  #pragma omp master
  {
     printf("\nTempo impiegato: %lf secondi\n\n", end_time - start_time);
     printVector(Risultato, M);
  }
   
  printf("\n");
  
  deallocateVector(Vettore);
  deallocateVector(Risultato);
  deallocateMatrix(Matrice, N);
   
}
