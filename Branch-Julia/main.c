#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> 

//tamanho máximo 5x4
#define COLUNA (5)
#define CARTELA (4)

int conteNumeroLinhas(FILE *arquivo) {
    char c, letra = '\n';
    int linhas = 0;
    
    while(fread (&c, sizeof(char), 1, arquivo)) {
        if(c == letra) {
          linhas++;
        }
    }
    rewind(arquivo);

    return linhas;
}

void monteCartela(FILE *arquivo, int linha_sel) {  
    rewind(arquivo);
    
    char linha[100];
    int cont = 1;
    
    while(fgets(linha, sizeof(linha), arquivo) != NULL) {
    
        if(cont == linha_sel) {
            linha[strcspn(linha, "\n")] = '\0';
            printf("%s",linha);
        }
        cont++;
    }
}

int numerosRepetidos(int nums_sorteado[], int qtd, int num) {
    for(int i = 0; i < qtd; i++) {
        if(nums_sorteado[i] == num) {
            return 1;
        }
    }
    
    return 0;
}

void sorteiaCartela(FILE *arquivo, int linhas) {
    int linha_sel = 0;
    int nums_sorteado[4];
    
    for(int i = 0; i < 4; i++) {
        do {
          linha_sel = rand() % linhas + 1;
          
        } while(numerosRepetidos(nums_sorteado, i, linha_sel));
        
        nums_sorteado[i] = linha_sel;
        printf("Cartela%2d: ", (i + 1));
        
        monteCartela(arquivo, linha_sel);
        
        if (i == 0) {
            printf("   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n");
        } else {
            printf("\n\n");
        }
    }
}

int main() {
    srand(time(NULL));
  
    FILE *arquivo;
    arquivo = fopen("cartelas.txt","r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int linhas = conteNumeroLinhas(arquivo);
    sorteiaCartela(arquivo, linhas);
    
    printf("# SELECIONE OUTRA CARTELA (2, 3 ou 4) ou PRESSIONE ENTER PARA SORTEAR: __");
    
    fclose(arquivo);
}