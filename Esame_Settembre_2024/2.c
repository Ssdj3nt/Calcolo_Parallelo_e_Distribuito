

/*
    Traccia
    1: il cor master deve generare una matriceC NxN
    2: calcolare in parallelo la triasposta della matriceC producendo una nuova matrice 
    3: i core devono collaborare per estrarre la diagonale dalla matice trasposta e inserirlo in un vettore D di lunghezza N


*/



#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>



// Definizione delle funzioni usate
void populateMatrix(int **matrix, int row, int col, int randModule);
void allocateMatrix(int ***matrix, int rows, int cols);
void allocateVector(int **vector, int size);
void printMatrix(int** matrix, int rows, int cols, char name[]);
void printVector(int *vector, int size, char name[]);
void freeMatrix(int **matrix, int rows);
double vectorNorma(int **vector, int size);



int main(){

    // Definizione di vettori e matrici

    int N, NP;
  
    int **matriceC;

    int *vectorD;
   
    int **traspostaC;

     printf( "\nInserisci il numero di thread: " );
	scanf( "%d", &NP );
	
	printf( "\nInserisci il numero di righe e colonne: " );
	scanf( "%d", &N );
	
    //1: il cor master deve generare una matriceC NxN
    // Il master inizializza e popola i vettori e le matrici
    #pragma omp master
    {

        // Matrice
        allocateMatrix(&matriceC, N, N);
        populateMatrix(matriceC, N, N, 10);

        // Stampa della matrice
        printMatrix(matriceC, N, N,"Matrice c");
        

    }

    //Variabile che conserva il tempo di inizio
    double startingTimer = omp_get_wtime();


    // 2: calcolare in parallelo la triasposta della matriceC producendo una nuova matrice
    // Sezione parallela
    #pragma omp parallel num_threads(NP) shared(matriceC, traspostaC, vectorD) 
    {
        
        
        allocateMatrix(&traspostaC, N, N);
    
        // Riempimento della matrice trasposta
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                traspostaC[j][i] = matriceC[i][j];
            }
        }

    
        #pragma omp master
        {
            printMatrix(traspostaC, N, N, "Matrice C trasposta" );
            
        }
        
    }
    

    // 3: i core devono collaborare per estrarre la diagonale dalla matice trasposta e inserirlo in un vettore D di lunghezza N

    allocateVector(&vectorD, N);
     #pragma omp parallel for
     for (int i = 0; i < N; i++)
         for(int j = 0; j < N; j++)
            (i == j ? vectorD[i] = traspostaC[i][j] : traspostaC[i][j]);
            printVector( vectorD,  N, "vettore diagonale");


    // Variabile che conserva il tempo di fine
    double endingtimer = omp_get_wtime();

    // Facciamo stampare al master il tempo di esecuzione
    #pragma omp master 
    {
           
            
       // double norma = vectorNorma(&vectorD, N);
        //printf("\nla norma del vettore D e': %.2f\n", norma)
        
        double totalTime = endingtimer - startingTimer;
        printf("\n********************\n\nTempo finale: %.7f\n", totalTime);
    }

    freeMatrix(matriceC, N);
    freeMatrix(traspostaC, N);



}


// Popolamento della matrice.
void populateMatrix(int **matrix, int row, int col, int randModule){
    for (int i = 0; i < row; i++)
        for(int j=0; j < col; j++)
            matrix[i][j] = rand() % randModule;
}


// Funzione per allocare la matrice
void allocateMatrix(int ***matrix, int rows, int cols) {
    *matrix = (int **) malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
        (*matrix)[i] = (int*) malloc(cols * sizeof(int));
}

// Funzione per allocare il vettore
void allocateVector(int **vector, int size){
    *vector = (int*) malloc( size * sizeof(int));
}


// Per questa funzione basta inviare il nome della matrice.
// Non serve l'indirizzo '&'.
void printMatrix(int** matrix, int rows, int cols, char name[]) {
    printf("\n%s:\n", name);
    // Ciclo le righe
    for (int i = 0; i < rows; i++){
        // Ciclo le colonne
        for (int j = 0; j < cols; j++)
            printf("%d\t", matrix[i][j]);
        // Dopo ogni fine colonna vado a capo
        printf("\n");
    }
}

// Funzione per stampare il vettore
void printVector(int *vector, int size, char name[]) {
    printf("\n%s:\n", name);
    for (int i = 0; i < size; i++) {
        printf("%d\t", vector[i]);
    }
    printf("\n");
}

// Funzione per liberare la memoria di una matrice
void freeMatrix(int **matrix, int rows) {
    for (int i = 0; i < rows; i++) 
        free(matrix[i]);

    free(matrix);
}

/*
// Funzione per fare la norma di un vettore 
double vectorNorma(int **vector, int size){
    double somma = 0.0;
     for (int i = 0; i < size; i++) {
        somma += abs((*vector)[i]);
    }
return somma;

}
*/


