#include <stdio.h>    // Biblioteca padrão de entrada e saída
#include <stdlib.h>   // Biblioteca para funções utilitárias (malloc, rand, etc.)
#include <string.h>   // Biblioteca para manipulação de strings
#include <time.h>     // Biblioteca para trabalhar com data e hora
 
#define MAX_CARTELAS 1000     // Número máximo de cartelas suportadas
#define NUM_POR_CARTELA 5     // Quantidade de números por cartela
#define CARTELAS_SORTEADAS 4  // Quantidade de cartelas sorteadas para jogar
#define MAX_LINHA 200         // Tamanho máximo de uma linha do arquivo
 
typedef struct {                 // Estrutura que representa uma cartela de bingo
    int nums[NUM_POR_CARTELA];   // Números da cartela
    int marked[NUM_POR_CARTELA]; // Marcações (0 = não marcado, 1 = marcado)
} Cartela;
 
// Função para ler as cartelas do arquivo
int ler_cartelas(const char *nome_arquivo, Cartela *lista, int max_cartelas) {
    FILE *f = fopen(nome_arquivo, "r");    // Abre o arquivo de cartelas
    if (!f) {                              // Verifica se falhou ao abrir
        return -1;                         // Retorna erro se não abriu
    }
    char linha[MAX_LINHA];                 // Buffer para armazenar cada linha
    int cont = 0;                          // Contador de cartelas lidas
 
    while (fgets(linha, sizeof(linha), f) && cont < max_cartelas) { // Lê cada linha
        for (char *p = linha; *p; p++) {   // Percorre a linha
            if (*p == ',') {               // Se encontrar vírgula
                *p = ' ';                  // Substitui por espaço
            } 
        }

        char *p = linha;                   // Ponteiro para ignorar espaços iniciais
        while (*p == ' ' || *p == '\t') p++; // Ignora espaços/tabs iniciais
        if (*p == '\n' || *p == '\0') continue; // Pula linhas vazias
 
        int a,b,c,d,e;                     // Variáveis para armazenar os números
        int lidos = sscanf(linha, "%d %d %d %d %d", &a,&b,&c,&d,&e); // Lê 5 números
        if (lidos != NUM_POR_CARTELA) continue;  // Se não leu 5, pula a linha
 
        lista[cont].nums[0] = a;           // Armazena os números na cartela
        lista[cont].nums[1] = b;
        lista[cont].nums[2] = c;
        lista[cont].nums[3] = d;
        lista[cont].nums[4] = e;
 
        for (int i = 0; i < NUM_POR_CARTELA; i++) { // Inicializa marcações
            lista[cont].marked[i] = 0;    // Nenhum número marcado no início
        }
        cont++;                            // Incrementa quantidade de cartelas lidas
    }
 
    fclose(f);                             // Fecha o arquivo
    return cont;                           // Retorna número de cartelas lidas
}
 
// Função para mostrar as cartelas na tela
void mostrar_cartelas(Cartela cartelas[], int indices[], int owned_idx, int cartela_vencedora) {
    printf("\n");                          // Linha em branco
 
    for (int i = 0; i < CARTELAS_SORTEADAS; i++) { // Percorre cada cartela
        int idx = indices[i];              // Índice da cartela atual
        if(cartela_vencedora != 0 && cartela_vencedora == i + 1) { // Se for a vencedora
            printf("**********");          // Bordas de destaque
            for (int j = 0; j < NUM_POR_CARTELA; j++) {
                printf(" **** ");
            }
            printf("\n");

            // Linha dos números
            printf("Cartela %d: ", i + 1); // Mostra números da cartela
            for (int j = 0; j < NUM_POR_CARTELA; j++) {
                printf("* %2d *", cartelas[idx].nums[j]);
            }
            if (i == owned_idx) {          // Marca se é a cartela do jogador
                printf(" <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
            } else {
                printf("\n");
            }
            // Linha de baixo
            printf("**********");          // Bordas inferiores
            for (int j = 0; j < NUM_POR_CARTELA; j++) {
                    printf(" **** ");
            }
            printf("\n");
        } else {                           // Caso não seja vencedora
            // Linha de cima
            printf("           ");
            for (int j = 0; j < NUM_POR_CARTELA; j++) {
                if (cartelas[idx].marked[j]) {
                    printf(" ---- ");      // Indica número marcado
                } else {
                    printf("      ");      // Espaço vazio
                }
            }
            printf("\n");
            // Linha dos números
            printf("Cartela %d: ", i + 1);
            for (int j = 0; j < NUM_POR_CARTELA; j++) {
                printf("| %2d |", cartelas[idx].nums[j]); // Mostra cada número
            }
            if (i == owned_idx) {          // Marca a cartela do jogador
                printf(" <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
            } else {
                printf("\n");
            }
            // Linha de baixo
            printf("           ");
            for (int j = 0; j < NUM_POR_CARTELA; j++) {
                if (cartelas[idx].marked[j]) {
                    printf(" ---- ");      // Linha inferior para número marcado
                } else {
                    printf("      ");      // Espaço vazio
                }
            }
            printf("\n");
        }
        printf("\n");
    }
}
 
// Verifica se uma cartela já foi sorteada
int cartelasRepetidas(int nums_sorteado[], int qtd, int num) {
    for(int i = 0; i < qtd; i++) {         // Percorre números já sorteados
        if(nums_sorteado[i] == num) {
            return 1;                      // Retorna 1 se repetido
        }
    }
    return 0;                              // Retorna 0 se não repetido
}
 
// Sorteia cartelas sem repetir
void sorteiaCartela(int linhas, int cartelas_sorteadas[]) {
    int linha_sel = 0;                      // Variável para linha selecionada
    for(int i = 0; i < CARTELAS_SORTEADAS; i++) {
        do {
          linha_sel = rand() % linhas + 1; // Escolhe uma cartela aleatória
        } while(cartelasRepetidas(cartelas_sorteadas, i, linha_sel)); // Evita repetição
        cartelas_sorteadas[i] = linha_sel; // Armazena cartela sorteada
    }
}
 
// Marca número sorteado nas cartelas
void marcar_numero(Cartela cartelas[], int indices[], int numero) {
    for (int i = 0; i < CARTELAS_SORTEADAS; i++) {
        int idx = indices[i];
        for (int j = 0; j < NUM_POR_CARTELA; j++) {
            if (cartelas[idx].nums[j] == numero) {
                cartelas[idx].marked[j] = 1; // Marca o número
            }
        }
    }
}
 
// Verifica se uma cartela está completa
int esta_completa(Cartela *c) {
    for (int i=0;i<NUM_POR_CARTELA;i++) {
        if (!c->marked[i]) {               // Se encontrar número não marcado
            return 0;                      // Não está completa
        } 
    }
    return 1;                              // Todos marcados → completa
}
 
// Sorteia um número único do pool
int sortear_numero_unico(int pool[], int *pool_size) {
    if (*pool_size == 0) { 
        return -1;                         // Nenhum número restante
    }
    int pos = rand() % (*pool_size);       // Escolhe posição aleatória
    int val = pool[pos];                   // Valor sorteado
    for (int i=pos;i<(*pool_size)-1;i++) {
        pool[i] = pool[i+1];               // Remove número do pool (compacta)
    } 
    (*pool_size)--;                        // Diminui tamanho do pool
    return val;                            // Retorna número sorteado
}
 
// Grava o vencedor no arquivo com data e hora
void gravar_vencedor(const char *nome_arquivo, const char *nome_vencedor) {
    FILE *f = fopen(nome_arquivo, "a");    // Abre arquivo para adicionar
    if (!f) {
        return;                            // Se falhar, retorna
    } 
    time_t t = time(NULL);                 // Obtém tempo atual
    struct tm tm = *localtime(&t);         // Converte para estrutura tm
    char timestr[64];                      // Buffer para data/hora formatada
    strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", &tm); // Formata data/hora
    fprintf(f, "%s - %s\n", timestr, nome_vencedor); // Escreve no arquivo
    fclose(f);                             // Fecha o arquivo
}
 
// Verifica vitória das cartelas
int verificar_vitoria(Cartela todas[], int cartelas_sorteadas[], int *jogo_ativo) {
    for (int i = 0; i < CARTELAS_SORTEADAS; i++) {
        if (esta_completa(&todas[cartelas_sorteadas[i]])) {
            *jogo_ativo = 0;               // Finaliza jogo
            return i + 1;                  // Retorna cartela vencedora
        }
    } 
    return 0;                              // Ninguém venceu ainda
}
 
// Anuncia vitória e grava vencedor
void anunciar_vitoria(int *jogo_ativo, int cartela_vencedora) {
    char entrada[100];                     // Buffer para nome do vencedor
    size_t ln;
    if (cartela_vencedora != 0) {          // Se houve vencedor
        printf("PARABÉNS! VOCÊ VENCEU!\nEntre o seu nome para constar no rol de vencedores: ");
        if (!fgets(entrada, sizeof(entrada), stdin)) {
            strcpy(entrada,"ANONIMO");     // Usa "ANONIMO" se falhar
        } 
        ln = strlen(entrada);
        if (ln>0 && entrada[ln-1]=='\n') {
            entrada[ln-1] = '\0';          // Remove newline
        } 
        if (strlen(entrada)==0) {
            strcpy(entrada,"ANONIMO");     // Usa "ANONIMO" se vazio
        }
        gravar_vencedor("vencedores.txt", entrada); // Grava vencedor
        printf("Nome registrado em vencedores.txt.\n");
    }
}
 
// Função principal
int main() {
    srand((unsigned) time(NULL));          // Inicializa gerador de números aleatórios
    Cartela todas[MAX_CARTELAS];           // Array de todas as cartelas
    int total_cartelas = ler_cartelas("cartelas.txt", todas, MAX_CARTELAS); // Lê cartelas
    if (total_cartelas <= 0) {
        printf("Erro: nao foi possivel ler cartelas.txt ou arquivo vazio/mal formatado.\n");
        return 1;                          // Sai se erro
    }
    if (total_cartelas < CARTELAS_SORTEADAS) {
        printf("Erro: cartelas.txt deve conter pelo menos %d cartelas.\n", CARTELAS_SORTEADAS);
        return 1;                          // Sai se poucas cartelas
    }
    int cartelas_sorteadas[CARTELAS_SORTEADAS]; // Guarda índices sorteados
    sorteiaCartela(total_cartelas, cartelas_sorteadas); // Sorteia cartelas
 
    int owned = 0;                         // Cartela do jogador (primeira)
    int jogo_ativo = 1;                    // Controle do loop do jogo
    int cartela_vencedora = 0;             // Indica cartela vencedora
 
    int pool[50];                          // Pool de números (1 a 50)
    int pool_size = 50;                    // Tamanho inicial do pool
    for (int i=0;i<50;i++) pool[i] = i+1;  // Preenche o pool
 
    printf("==== JOGO DE BINGO ====\n");    // Titulo do jogo
    mostrar_cartelas(todas, cartelas_sorteadas, owned, cartela_vencedora); // Mostra cartelas
 
    char entrada[100];                     // Buffer para entrada do usuário
 
    while (jogo_ativo) {                   // Loop do jogo
        printf("# SELECIONE OUTRA CARTELA (1, 2 ou 4) ou PRESSIONE ENTER PARA SORTEAR: ");
        if (!fgets(entrada, sizeof(entrada), stdin)) {
            break;                         // Sai do loop se entrada falhar
        } 
        size_t ln = strlen(entrada);
        if (ln>0 && entrada[ln-1]=='\n') {
            entrada[ln-1] = '\0';          // Remove newline
        }
        if (strlen(entrada) == 0) {        // Se pressionou ENTER
            if (pool_size == 0) {
                break;                     // Fim do jogo se acabou números
            }
            int numero = sortear_numero_unico(pool, &pool_size); // Sorteia número
            printf("\n##########################################################################\n");
            printf("#                           NÚMERO SORTEADO: %d                          #\n", numero);
            printf("##########################################################################\n");
            marcar_numero(todas, cartelas_sorteadas, numero);   // Marca número nas cartelas
            cartela_vencedora = verificar_vitoria(todas, cartelas_sorteadas, &jogo_ativo); // Verifica vitória
            mostrar_cartelas(todas, cartelas_sorteadas, owned, cartela_vencedora);         // Atualiza exibição
            
            if (jogo_ativo == 0) { // Se o jogo acabou

            if (cartela_vencedora == owned + 1) {  // Se a cartela vencedora é a do jogador
                anunciar_vitoria(&jogo_ativo, cartela_vencedora);  // Anuncia a vitória do jogador
            } else {
                printf("# OUTRA CARTELA FOI COMPLETADA!\n");  // Outra cartela venceu
                 printf("# Melhor sorte na próxima vez!\n");  // Mensagem de consolo
            }
}
            
        } else {                          // Se digitou troca de cartela
            int escolha;
            if (sscanf(entrada, "%d", &escolha)!=1 || escolha<1 || escolha>4) {
                printf("Entrada inválida.\n");
                continue;                 // Pede novamente se inválido
            }
            if (escolha-1==owned) {
                printf("Você já possui essa cartela.\n");
                continue;                 // Se já possui, ignora
            }
            owned = escolha-1;            // Atualiza cartela do jogador
            printf("\n##########################################################################\n");
            printf("#                       VOCÊ AGORA É DONO DA CARTELA: %d                  #\n", escolha);
            printf("##########################################################################\n");
            mostrar_cartelas(todas, cartelas_sorteadas, owned, cartela_vencedora); // Mostra cartelas
        }
    }
    return 0; // Fim do programa
}
