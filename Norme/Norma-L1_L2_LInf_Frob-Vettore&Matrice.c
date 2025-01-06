// Normalizzazione vettore con norma L1
// Normalizzazione vettore con norma L2
// Normalizzazione vettore con norma L-infinito

// Normalizzazione matrice con norma L1
// Normalizzazione matrice con norma Frobenius
// Normalizzazione matrice con norma L-infinito

// pow, sqrt, abs, fabs

#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "omp.h"
#include "time.h"

void allocateMatrix(double ***matrix, size_t n, size_t m) {
    *matrix = (double**)calloc(n, sizeof(double*));
    for (size_t i = 0; i < n; i++) {
        (*matrix)[i] = (double*)calloc(m, sizeof(double));
    }
}

void deallocateMatrix(double **matrix, size_t n) {
    for (size_t i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void fillMatrix(double **Matrix, size_t n, size_t m) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            Matrix[i][j] = rand() % 9;
        }
    }
}

void printMatrix(double **Matrix, size_t n, size_t m) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            printf("%f ", Matrix[i][j]);
        }
        printf("\n");
    }
}

void allocateVector(double **vector, size_t m) {
    *vector = (double*)calloc(m, sizeof(double));
}

void deallocateVector(double *vector) {
    free(vector);
}

void printVector(double *vector, size_t m) {
    for (size_t i = 0; i < m; i++) {
        printf("%f ", vector[i]);
    }
    printf("\n");
}

void fillVector(double *vector, size_t m) {
    for (size_t i = 0; i < m; i++) {
        vector[i] = rand() % 9;
    }
}

int main() {

    double *Vector, **Matrix;
    double *DummyV, **DummyM;
    double normaV_L1 = 0, normaM_L1 = 0;
    double normaV_L2 = 0, normaM_Frob = 0;
    double normaV_LInf = 0, normaM_Linf = 0;
    size_t N, M;
    int np;

    srand(time(NULL));

    printf("\nSize N:\n");
    scanf("%zu", &N);
    printf("\nSize M:\n");
    scanf("%zu", &M);
    printf("\nNumber of Core:\n");
    scanf("%d", &np);

    allocateVector(&Vector, N);
    allocateVector(&DummyV, N);
    fillVector(Vector, N);

    allocateMatrix(&Matrix, N, M);
    allocateMatrix(&DummyM, N, M);
    fillMatrix(Matrix, N, M);

    printf("\nVector\n");
    printVector(Vector, N);
    
    // Norma L1 Vettore
    #pragma omp parallel for num_threads(np) reduction(+: normaV_L1)
    for(size_t i = 0; i < N; i++) {
       normaV_L1 += fabs(Vector[i]);
    }
    
    printf("\nL1 vector norm: %f\n", normaV_L1);
    
    // Normalizzazione L1 Vettore
    #pragma omp parallel for num_threads(np)
    for (size_t i = 0; i < N; i++) {
       DummyV[i] = Vector[i] / normaV_L1;
    }
    
    printf("\nL1 normalized vector:\n");
    printVector(DummyV, N);
    
    // Norma L2 Vettore
    #pragma omp parallel for num_threads(np) reduction(+: normaV_L2)
    for (size_t i = 0; i < N; i++) {
        DummyV[i] = Vector[i] * Vector[i]; // oppure DummyV[i] = pow(Vector[i], 2);
        normaV_L2 += DummyV[i];
    }
    
    normaV_L2 = sqrt(normaV_L2);

    printf("\nL2 vector norm: %f\n", normaV_L2);
    
    // Normalizzazione L2 Vettore
    #pragma omp parallel for num_threads(np)
    for (size_t i = 0; i < N; i++) {
        DummyV[i] = Vector[i] / normaV_L2;
    }
    
    printf("\nL2 normalized vector:\n");
    printVector(DummyV, N);
    
    // Norma L-infinito vettore
    #pragma omp parallel for num_threads(np) reduction(max: normaV_LInf)
    for(size_t i = 0; i < N; i++) {
       normaV_LInf = fmax(normaV_LInf, fabs(Vector[i]));
    }
    
    printf("\nL-infinite vector norm: %f\n", normaV_LInf);
    
    // Normalizzazione L-Inf Vettore
    #pragma omp parallel for num_threads(np)
    for (size_t i = 0; i < N; i++) {
       DummyV[i] = Vector[i] / normaV_LInf;
    }
    
    printf("\nL-infinite normalized vector:\n");
    printVector(DummyV, N);
    
    // Norma L1 Matrice
    #pragma omp parallel for num_threads(np) reduction(max: normaM_L1)
    for (int j = 0; j < M; j++) {
        double sum = 0;
        for (int i = 0; i < N; i++) {
            sum += fabs(Matrix[i][j]);
        }
        normaM_L1 = fmax(normaM_L1, sum);
    }
    
    printf("\nMatrix\n");
    printMatrix(Matrix, N, M);
    
    printf("\nMatrix L1 Norm: %f\n", normaM_L1);
    
    // Normalizzazione L1 Matrice
    #pragma omp parallel for num_threads(np)
    for (size_t i = 0; i < N; i++) {
       for (size_t j = 0; j < M; j++) {
          DummyM[i][j] = Matrix[i][j] / normaM_L1;
       }
    }
    
    printf("\nL1 normalized matrix:\n");
    printMatrix(DummyM, N, M);
    
    // Norma Frobenius Matrice
    #pragma omp parallel for num_threads(np) reduction(+: normaM_Frob)
    for (size_t i = 0; i < N; i++) {
       for (size_t j = 0; j < M; j++) {
          DummyM[i][j] = fabs(Matrix[i][j]) * fabs(Matrix[i][j]);
          normaM_Frob += DummyM[i][j];
       }
    }
    
    normaM_Frob = sqrt(normaM_Frob);
    
    printf("\nMatrix Frobenius Norm: %f\n", normaM_Frob);
    
    // Normalizzazione Frobenius Matrice
    #pragma omp parallel for num_threads(np)
    for (size_t i = 0; i < N; i++) {
       for (size_t j = 0; j < M; j++) {
          DummyM[i][j] = Matrix[i][j] / normaM_Frob;
       }
    }
    
    printf("\nFrobenius normalized matrix:\n");
    printMatrix(DummyM, N, M);
    
    // Norma L-Infinita Matrice
    #pragma omp parallel for num_threads(np) reduction(max: normaM_Linf)
    for (int i = 0; i < N; i++) {
        double sum = 0;
        for (int j = 0; j < M; j++) {
            sum += fabs(Matrix[i][j]);
        }
        normaM_Linf = fmax(normaM_Linf, sum);
    }
    
    printf("\nMatrix L-Infinite Norm: %f\n", normaM_Linf);
    
    // Normalizzazione L-Inf Matrice
    #pragma omp parallel for num_threads(np)
    for (size_t i = 0; i < N; i++) {
       for (size_t j = 0; j < M; j++) {
          DummyM[i][j] = Matrix[i][j] / normaM_Linf;
       }
    }
    
    printf("\nL-Inf normalized matrix:\n");
    printMatrix(DummyM, N, M);
    
    deallocateVector(Vector);
    deallocateVector(DummyV);
    deallocateMatrix(Matrix, N);
    deallocateMatrix(DummyM, N);
    
    printf("\n");

}

