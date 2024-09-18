1) Prodotto esterno matrice, dai due vettori a e b;
2) Prodotto matrice e vettore;
3) Tempi

#include "stdio.h"
#include "stdlib.h"
#include "omp.h"

void fillVectorA(int *vector, int size){
for(size_t i = 0; i < size; i++){
vector[i] = 1;
}
}

void fillVectorBC(int *vector, int size){
for(size_t i = 0; i < size; i++){
vector[i] = 2;
}
}

void printVector(int *vector, int size){
for(size_t i = 0; i < size; i++){
printf("%3d",vector[i]);
}
}

int main(void){
int *a,*b,*c;
int **A;
int N,M;
double start_time = 0;
double end_time = 0;

printf("\n\nEsame Settembre 2024 - Francesco Porritiello 0124002486\n\n");

printf("\nDefinisci N:\n");
scanf("%d",&N);
printf("\nDefinisci M:\n");
scanf("%d",&M);
int *Result = calloc(N, sizeof(int));

start_time = omp_get_wtime();
#pragma omp master
{
a = (int*)calloc(N,sizeof(int));
b = (int*)calloc(M,sizeof(int));
c = (int*)calloc(M,sizeof(int));
fillVectorA(a,N);
fillVectorBC(b,M);
fillVectorBC(c,M);
}

printf("\nVettore a:");
printVector(a,N);
printf("\n\nVettore b:");
printVector(b,M);
printf("\n\nVettore c:");
printVector(c,M);
printf("\n");

// Allocazione Matrice A
A = (int**)calloc(N,sizeof(int*));
for (size_t i = 0; i < N; i++) {
A[i] = (int*)calloc(M, sizeof(int));
}

#pragma parallel for shared(A,a,b)
{
for(size_t i = 0; i < N; i++){
 for(size_t j = 0; j < M; j++){
A[i][j] = a[i]*b[j];
}
}
}

#pragma omp parallel for shared(A,c)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            Result[i] += A[i][j] * c[j];
        }
    }

end_time = omp_get_wtime();

printf("\nMatrice A:\n");
printf("\n");
for(size_t i = 0; i < N; i++){
 for(size_t j = 0; j < M; j++){
printf("%d ",A[i][j]);
}
printf("\n");
}
printf("\n");

#pragma omp master
{
printf("\nVettore Risultato:\n");
printVector(Result,N);
printf("\n\n");
printf("Tempo impiegato: %lf secondi\n\n", end_time - start_time);
}

//Deallocazione Matrice A ed Array a,b,c,Result
free(a);
free(b);
free(c);
free(Result);

for(size_t i = 0; i < N; i++){
free(A[i]);
}
free(A);

}
