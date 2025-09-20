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
    int marked[NUM_POR_CARTELA]; // 0 ou 1
} Cartela;

int ler_cartelas(const char *nome_arquivo, Cartela *lista, int max_cartelas) {
    FILE *f = fopen(nome_arquivo, "r");
    if (!f) return -1;
    char linha[MAX_LINHA];
    int cont = 0;

    while (fgets(linha, sizeof(linha), f) && cont < max_cartelas) {

        int a,b,c,d,e;
        int lidos = sscanf(linha, "%d %d %d %d %d", &a,&b,&c,&d,&e);
        if (lidos != NUM_POR_CARTELA) continue;

        lista[cont].nums[0] = a;
        lista[cont].nums[1] = b;
        lista[cont].nums[2] = c;
        lista[cont].nums[3] = d;
        lista[cont].nums[4] = e;
        for (int i=0; i<NUM_POR_CARTELA; i++) lista[cont].marked[i] = 0;
        cont++;
    }

    fclose(f);
    return cont;
}

void mostrar_cartelas(Cartela cartelas[], int indices[], int owned_idx) {
    printf("\n");
    for (int i=0;i<CARTELAS_SORTEADAS;i++) {
        int idx = indices[i];
        printf("\033[33mCartela %d: \033[0m", i+1);
        for (int j=0;j<NUM_POR_CARTELA;j++) {
             if (cartelas[idx].marked[j]) {
                // Número marcado, imprime em verde
                printf("\033[32m*%2d*\033[0m ", cartelas[idx].nums[j]);
            } else {
                printf(" %2d  ", cartelas[idx].nums[j]);
            }
        }
        if (i == owned_idx) printf(" <<<<<<< VOCÊ");
        printf("\n");
    }
    printf("\n");
}

void marcar_numero(Cartela cartelas[], int indices[], int numero) {
    for (int i=0;i<CARTELAS_SORTEADAS;i++) {
        int idx = indices[i];
        for (int j=0;j<NUM_POR_CARTELA;j++) {
            if (cartelas[idx].nums[j] == numero) cartelas[idx].marked[j] = 1;
        }
    }
}

int esta_completa(Cartela *c) {
    for (int i=0;i<NUM_POR_CARTELA;i++)
        if (!c->marked[i]) return 0;
    return 1;
}

int sortear_numero_unico(int pool[], int *pool_size) {
    if (*pool_size == 0) return -1;
    int pos = rand() % (*pool_size);
    int val = pool[pos];
    for (int i=pos;i<(*pool_size)-1;i++) pool[i] = pool[i+1]; // compacta pool
    (*pool_size)--;
    return val;
}

void gravar_vencedor(const char *nome_arquivo, const char *nome_vencedor) {
    FILE *f = fopen(nome_arquivo, "a");
    if (!f) return;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char timestr[64];
    strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", &tm);
    fprintf(f, "%s - %s\n", timestr, nome_vencedor);
    fclose(f);
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

    // Escolher 4 cartelas distintas
    int indices[CARTELAS_SORTEADAS];
    for (int i=0;i<CARTELAS_SORTEADAS;i++) {
        int r, ok;
        do {
            r = rand() % total_cartelas;
            ok = 1;
            for (int j=0;j<i;j++) if (indices[j]==r) { ok=0; break; }
        } while(!ok);
        indices[i] = r;
    }

    int owned = 0; // Jogador começa com cartela 1

    // Criar pool de números únicos (1 a 50)
    int pool[50];
    int pool_size = 50;
    for (int i=0;i<50;i++) pool[i] = i+1;

    printf("==== JOGO DE BINGO ====\n");
    mostrar_cartelas(todas, indices, owned);

    char entrada[100];
    int jogo_ativo = 1;

    while (jogo_ativo) {
        printf("# Troque de cartela (1-4) ou pressione ENTER para sortear: ");
        if (!fgets(entrada, sizeof(entrada), stdin)) break;
        size_t ln = strlen(entrada);
        if (ln>0 && entrada[ln-1]=='\n') entrada[ln-1] = '\0';

        if (strlen(entrada)==0) {
            if (pool_size==0) {
                printf("Todos os numeros foram sorteados. Fim do jogo.\n");
                break;
            }
            int numero = sortear_numero_unico(pool, &pool_size);
            printf("\n==============================\n");
            printf(" NÚMERO SORTEADO: \033[33m|%d|\n\033[0m", numero);
            printf("==============================\n");
            marcar_numero(todas, indices, numero);
            mostrar_cartelas(todas, indices, owned);

            // Verificar vitória
            for (int i=0;i<CARTELAS_SORTEADAS;i++) {
                if (esta_completa(&todas[indices[i]])) {
                    if (i==owned) {
                        printf("\033[33mPARABÉNS! VOCÊ VENCEU!\n\033[0m\nDigite seu nome: ");
                        if (!fgets(entrada, sizeof(entrada), stdin)) strcpy(entrada,"ANONIMO");
                        ln = strlen(entrada);
                        if (ln>0 && entrada[ln-1]=='\n') entrada[ln-1] = '\0';
                        if (strlen(entrada)==0) strcpy(entrada,"ANONIMO");
                        gravar_vencedor("vencedores.txt", entrada);
                        printf("Nome registrado em vencedores.txt.\n");
                    } else {
                        printf("Outra cartela completou primeiro. Fim do jogo!\n");
                    }
                    jogo_ativo = 0;
                    break;
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
            printf("Agora você é dono da Cartela %d.\n", escolha);
            mostrar_cartelas(todas, indices, owned);
        }
    }

    printf("Fim do jogo. Obrigado por jogar!\n");
    return 0;
}
