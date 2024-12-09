
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

// Definizione delle funzioni usate
double normaVettore(double **matrice, int lunghezzaColonna, int colDaSommare);
void populateMatrix(double **matrix, int row, int col, int randModule);
void allocateMatrix(double ***matrix, int rows, int cols);
void printMatrix(double** matrix, int rows, int cols, char name[]);
void freeMatrix(double **matrix, int rows);


int main(){

    // Definizione di vettori e matrici
    double **matriceC;
    double **C_norm;
    // Definizioni delle dimensioni
    int N, M;
    // Numero dei threads
    int threadNumber;
    // Variabile per conservare la somma della prima colonna della matrice normalizzata
    double endResult = 0.0;

    // Il master inizializza e popola i vettori e le matrici
    #pragma omp master
    {

        // --- PUNTO 1 ---
        printf("Inserisci il numero di righe N: ");
        scanf("%d", &N);
        printf("Inserisci il numero di colonne M: ");
        scanf("%d", &M);
        printf("Inserisci il numero di THREADS: ");
        scanf("%d", &threadNumber);

        // Allocazione delle matrici C e C-norm
        allocateMatrix(&matriceC, N, M);
        allocateMatrix(&C_norm, N, M);
        populateMatrix(matriceC, N, M, 10);
        // Con il quarto argomento della funzione
        // scelgo il modulo da applicare alla funzione rand
        // potendola inizializzare a 0
        populateMatrix(C_norm, N, M, 1);

        // Stampa delle matrici
        printMatrix(matriceC, N, M, "Matrice C");

    }

    //Variabile che conserva il tempo di inizio
    double startingTimer = omp_get_wtime();

    // Sezione parallela
    #pragma omp parallel num_threads(threadNumber) shared(matriceC, C_norm, threadNumber, N, M)
    {

        // --- PUNTO 2 ---

        //Distribuzione delle colonne per i processori
        int colonneLocali = M / threadNumber;
        int resto = M % threadNumber;
        int colonnaDiInizio = omp_get_thread_num() * colonneLocali;

        // Con questo controllo andiamo a ridistribuire le colonne ai singoli thread
        // nel caso in cui le colonne non siano interamente divisibili per il numero di thread

        // I processori che hanno ID minore del resto ottengono una riga in più
        if (omp_get_thread_num() < resto){
            colonneLocali += 1;

        // Tutti gli altri si spostano di un numero di colonne pari al resto,
        // poiché gli indici di che avanzano sono stati ridistribuite ai core precedenti
        } else {
            colonnaDiInizio += resto;
        }


        // Andiamo a calcolare la normalizzazione della matrice C
        for(int i = 0; i < N; i++){
            for(int j = 0; j < colonneLocali; j++){

                // Formula per calcolare la normalizzazione
                // La funzione normaVettore mi permette di calcolare
                // la norma della colonna selezionata della matrice
                // DEBUGGING: printf("%.4f\n, i = %d, j = %d, N = %d, M = %d\n", matriceC[i][j + colonnaDiInizio] / sqrt(normaVettore(matriceC, N, j + colonnaDiInizio)), i, j, N, M);
                C_norm[i][j + colonnaDiInizio] = matriceC[i][j + colonnaDiInizio] / sqrt(normaVettore(matriceC, N, j + colonnaDiInizio));

            }

        }

    }

     printf("\nFinita la sezione parallela\n");

    // Facciamo stampare al master il tempo di esecuzione
    #pragma omp master
    {

        // --- PUNTO 3 ---

        // Il core master effettua la somma degli elementi della prima colonna
        // della matrice C normalizzata
        for(int i = 0; i < M; i++){
            endResult += C_norm[0][i];
        }

        // Variabile che conserva il tempo di fine
        double endingtimer = omp_get_wtime();
        double totalTime = endingtimer - startingTimer;

        printMatrix(C_norm, N, M, "Normalizzazione della matrice C");
        printf("\n\nSomma totale degli elementi della prima riga di C_norm: %.2f", endResult);
        printf("\n\nTEMPO TOTALE PER L'ESECUZIONE: %.7f\n\n", totalTime);

        // Liberiamo le memorie
        freeMatrix(matriceC, N);
        freeMatrix(C_norm, N);
    }




}

// Questa funzione calcola la norma di un vettore
// colonna scelto dalla matrice in ingresso
double normaVettore(double **matrice, int lunghezzaColonna, int colDaSommare){

    // Variabile che conserva il valore della norma
    double norma = 0;

    // Questo ciclo for scorre le righe della colonna scelta
    // e per ogni componente ne esegue il quadrato e ne somma il valore al totale
    for(int k = 0; k< lunghezzaColonna; k++){
        norma += pow(matrice[k][colDaSommare], 2);

    }

    // Alla fine viene ritornato il valor della norma
    return norma;

}


// Popolamento della matrice.
void populateMatrix(double **matrix, int row, int col, int randModule){
    for (int i = 0; i < row; i++)
        for(int j=0; j < col; j++)
            matrix[i][j] = rand() % randModule;
}

// Funzione per allocare la matrice
void allocateMatrix(double ***matrix, int rows, int cols) {
    *matrix = (double **) malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++)
        (*matrix)[i] = (double*) malloc(cols * sizeof(double));
}


// Per questa funzione basta inviare il nome della matrice.
// Non serve l'indirizzo '&'.
void printMatrix(double** matrix, int rows, int cols, char name[]) {
    printf("\n%s:\n", name);
    // Ciclo le righe
    for (int i = 0; i < rows; i++){
        // Ciclo le colonne
        for (int j = 0; j < cols; j++)
            printf("%.1f\t", matrix[i][j]);
        // Dopo ogni fine colonna vado a capo
        printf("\n");
    }
}


// Funzione per liberare la memoria di una matrice
void freeMatrix(double **matrix, int rows) {
    for (int i = 0; i < rows; i++)
        free(matrix[i]);

    free(matrix);
}
