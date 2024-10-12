//Approssimazione del π utilizzando l'integrazione numerica con il metodo del rettangolo.

#include <stdio.h>
#include <omp.h>

#define N 10000000

int main(int argc, char **argv) {
    long int i, n = N;
    double x, dx, f, sum, pi;
    printf("numero di intervalli: %ld\n", n);

    sum = 0.0;
    dx = 1.0 / (double)n;

#pragma omp parallel for private(x, f, i) shared(dx, sum, n)
    for (i = 1; i <= n; i++) {  // Qui correggiamo la condizione del ciclo for
        x = dx * ((double)(i - 0.5));
        f = 4.0 / (1.0 + x * x);

#pragma omp critical
        sum += f;
    }

    pi = dx * sum;
    printf("Pi = %.24f\n", pi);
    return 0;
}
