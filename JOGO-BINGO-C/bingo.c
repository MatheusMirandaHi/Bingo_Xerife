#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <time.h>  
 
#define MAX_CARTELAS 1000     
#define NUM_POR_CARTELA 5     
#define CARTELAS_SORTEADAS 4  
#define MAX_LINHA 200         
 
typedef struct {                 
    int nums[NUM_POR_CARTELA];   
    int marked[NUM_POR_CARTELA]; 
} Cartela;
 
int ler_cartelas(const char *nome_arquivo, Cartela *lista, int max_cartelas) {
    FILE *f = fopen(nome_arquivo, "r");    
    if (!f) {                             
        return -1;                         
    }
    char linha[MAX_LINHA];                 
    int cont = 0;                          
 
    while (fgets(linha, sizeof(linha), f) && cont < max_cartelas) { 
        for (char *p = linha; *p; p++) {   
            if (*p == ',') {               
                *p = ' ';                  
            } 
        }

        char *p = linha;                   
        while (*p == ' ' || *p == '\t') p++; 
        if (*p == '\n' || *p == '\0') continue; 
 
        int a,b,c,d,e;                     
        int lidos = sscanf(linha, "%d %d %d %d %d", &a,&b,&c,&d,&e); 
        if (lidos != NUM_POR_CARTELA) continue;  
 
        lista[cont].nums[0] = a;           
        lista[cont].nums[1] = b;
        lista[cont].nums[2] = c;
        lista[cont].nums[3] = d;
        lista[cont].nums[4] = e;
 
        for (int i = 0; i < NUM_POR_CARTELA; i++) { 
            lista[cont].marked[i] = 0;    
        }
        cont++;                            
    }
 
    fclose(f);                             
    return cont;                           
}
 
void mostrar_cartelas(Cartela cartelas[], int indices[], int owned_idx, int cartela_vencedora) {
    printf("\n");                          
 
    for (int i = 0; i < CARTELAS_SORTEADAS; i++) { 
        int idx = indices[i];              
        if(cartela_vencedora != 0 && cartela_vencedora == i + 1) { 
            printf("**********");          
            for (int j = 0; j < NUM_POR_CARTELA; j++) {
                printf(" **** ");
            }
            printf("\n");

            printf("Cartela %d: ", i + 1); 
            for (int j = 0; j < NUM_POR_CARTELA; j++) {
                printf("* %2d *", cartelas[idx].nums[j]);
            }
            if (i == owned_idx) {          
                printf(" <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
            } else {
                printf("\n");
            }
            
            printf("**********");          
            for (int j = 0; j < NUM_POR_CARTELA; j++) {
                    printf(" **** ");
            }
            printf("\n");
        } else {                           
            
            printf("           ");
            for (int j = 0; j < NUM_POR_CARTELA; j++) {
                if (cartelas[idx].marked[j]) {
                    printf(" ---- ");      
                } else {
                    printf("      ");      
                }
            }
            printf("\n");
            
            printf("Cartela %d: ", i + 1);
            for (int j = 0; j < NUM_POR_CARTELA; j++) {
                printf("| %2d |", cartelas[idx].nums[j]); 
            }
            if (i == owned_idx) {          
                printf(" <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
            } else {
                printf("\n");
            }
            
            printf("           ");
            for (int j = 0; j < NUM_POR_CARTELA; j++) {
                if (cartelas[idx].marked[j]) {
                    printf(" ---- ");      
                } else {
                    printf("      ");      
                }
            }
            printf("\n");
        }
        printf("\n");
    }
}
 
int cartelasRepetidas(int nums_sorteado[], int qtd, int num) {
    for(int i = 0; i < qtd; i++) {         
        if(nums_sorteado[i] == num) {
            return 1;                     
        }
    }
    return 0;                              
}
 
void sorteiaCartela(int linhas, int cartelas_sorteadas[]) {
    int linha_sel = 0;                      
    for(int i = 0; i < CARTELAS_SORTEADAS; i++) {
        do {
          linha_sel = rand() % linhas + 1; 
        } while(cartelasRepetidas(cartelas_sorteadas, i, linha_sel)); 
        cartelas_sorteadas[i] = linha_sel; 
    }
}

void marcar_numero(Cartela cartelas[], int indices[], int numero) {
    for (int i = 0; i < CARTELAS_SORTEADAS; i++) {
        int idx = indices[i];
        for (int j = 0; j < NUM_POR_CARTELA; j++) {
            if (cartelas[idx].nums[j] == numero) {
                cartelas[idx].marked[j] = 1; 
            }
        }
    }
}
 
int esta_completa(Cartela *c) {
    for (int i=0;i<NUM_POR_CARTELA;i++) {
        if (!c->marked[i]) {               
            return 0;                      
        } 
    }
    return 1;                              
}
 
int sortear_numero_unico(int pool[], int *pool_size) {
    if (*pool_size == 0) { 
        return -1;                         
    }
    int pos = rand() % (*pool_size);       
    int val = pool[pos];                   
    for (int i=pos;i<(*pool_size)-1;i++) {
        pool[i] = pool[i+1];               
    } 
    (*pool_size)--;                        
    return val;                            
}
 
void gravar_vencedor(const char *nome_arquivo, const char *nome_vencedor) {
    FILE *f = fopen(nome_arquivo, "a");    
    if (!f) {
        return;                            
    } 
    time_t t = time(NULL);                 
    struct tm tm = *localtime(&t);         
    char timestr[64];                      
    strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", &tm); 
    fprintf(f, "%s - %s\n", timestr, nome_vencedor); 
    fclose(f);                             
}
 
int verificar_vitoria(Cartela todas[], int cartelas_sorteadas[], int *jogo_ativo) {
    for (int i = 0; i < CARTELAS_SORTEADAS; i++) {
        if (esta_completa(&todas[cartelas_sorteadas[i]])) {
            *jogo_ativo = 0;               
            return i + 1;                  
        }
    } 
    return 0;                              
}
 
void anunciar_vitoria(int *jogo_ativo, int cartela_vencedora) {
    char entrada[100];                     
    size_t ln;
    if (cartela_vencedora != 0) {          
        printf("PARABÉNS! VOCÊ VENCEU!\nEntre o seu nome para constar no rol de vencedores: ");
        if (!fgets(entrada, sizeof(entrada), stdin)) {
            strcpy(entrada,"ANONIMO");     
        } 
        ln = strlen(entrada);
        if (ln>0 && entrada[ln-1]=='\n') {
            entrada[ln-1] = '\0';          
        } 
        if (strlen(entrada)==0) {
            strcpy(entrada,"ANONIMO");     
        }
        gravar_vencedor("vencedores.txt", entrada); 
        printf("Nome registrado em vencedores.txt.\n");
    }
}
 
int main() {
    srand((unsigned) time(NULL));          
    Cartela todas[MAX_CARTELAS];           
    int total_cartelas = ler_cartelas("cartelas.txt", todas, MAX_CARTELAS); 
    if (total_cartelas <= 0) {
        printf("Erro: nao foi possivel ler cartelas.txt ou arquivo vazio/mal formatado.\n");
        return 1;                          
    }
    if (total_cartelas < CARTELAS_SORTEADAS) {
        printf("Erro: cartelas.txt deve conter pelo menos %d cartelas.\n", CARTELAS_SORTEADAS);
        return 1;                          
    }
    int cartelas_sorteadas[CARTELAS_SORTEADAS]; 
    sorteiaCartela(total_cartelas, cartelas_sorteadas); 
 
    int owned = 0;                         
    int jogo_ativo = 1;                    
    int cartela_vencedora = 0;             
 
    int pool[50];                          
    int pool_size = 50;                    
    for (int i=0;i<50;i++) pool[i] = i+1;  
 
    printf("==== JOGO DE BINGO ====\n");    
    mostrar_cartelas(todas, cartelas_sorteadas, owned, cartela_vencedora); 
 
    char entrada[100];                     
 
    while (jogo_ativo) {                   
        printf("# SELECIONE OUTRA CARTELA (1, 2 ou 4) ou PRESSIONE ENTER PARA SORTEAR: ");
        if (!fgets(entrada, sizeof(entrada), stdin)) {
            break;                         
        } 
        size_t ln = strlen(entrada);
        if (ln>0 && entrada[ln-1]=='\n') {
            entrada[ln-1] = '\0';          
        }
        if (strlen(entrada) == 0) {       
            if (pool_size == 0) {
                break;                     
            }
            int numero = sortear_numero_unico(pool, &pool_size); 
            printf("\n##########################################################################\n");
            printf("#                           NÚMERO SORTEADO: %d                          #\n", numero);
            printf("##########################################################################\n");
            marcar_numero(todas, cartelas_sorteadas, numero);   
            cartela_vencedora = verificar_vitoria(todas, cartelas_sorteadas, &jogo_ativo); 
            mostrar_cartelas(todas, cartelas_sorteadas, owned, cartela_vencedora);         
            
            if (jogo_ativo == 0) { 

            if (cartela_vencedora == owned + 1) {  
                anunciar_vitoria(&jogo_ativo, cartela_vencedora);  
            } else {
                printf("# OUTRA CARTELA FOI COMPLETADA!\n");  
                 printf("# Melhor sorte na próxima vez!\n");  
            }
}
            
        } else {                          
            int escolha;
            if (sscanf(entrada, "%d", &escolha)!=1 || escolha<1 || escolha>4) {
                printf("Entrada inválida.\n");
                continue;                 
            }
            if (escolha-1==owned) {
                printf("Você já possui essa cartela.\n");
                continue;                
            }
            owned = escolha-1;            
            printf("\n##########################################################################\n");
            printf("#                       VOCÊ AGORA É DONO DA CARTELA: %d                  #\n", escolha);
            printf("##########################################################################\n");
            mostrar_cartelas(todas, cartelas_sorteadas, owned, cartela_vencedora); 
        }
    }
    return 0; 
}
