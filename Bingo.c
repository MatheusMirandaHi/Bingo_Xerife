// Imprime 50 combinações de jogos para a Mega Sena.
#include <stdio.h>
#include <stdlib.h>

//tamanho máximo 5x4
#define COLUNA (5)
#define CARTELA (4)

int main(){
int matriz[CARTELA][COLUNA]; //declaração da matriz

    for (int i = 0; i < CARTELA; i++)
    for (int j = 0; j < COLUNA; j++)

    matriz[i][j] = rand() % 50 + 1; // Números aleatórios de 1 a 50.

    for (int i = 0; i < CARTELA; i++){
        printf("Cartela%2d: | ", (i + 1));

    for (int j = 0; j < COLUNA; j++)
        printf("%2d | ", matriz[i][j]);
        printf("\n\n\n");
        
    }    
        
        printf("# SELECIONE OUTRA CARTELA (2, 3 ou 4) ou PRESSIONE ENTER PARA SORTEAR: __");
  
}