🎲 Bingo em C

Um jogo de Bingo interativo feito em C que sorteia cartelas aleatórias a partir de um arquivo (cartelas.txt), permite ao jogador escolher qual cartela controlar e realiza o sorteio de números até que uma cartela seja completada. O vencedor pode registrar seu nome em um arquivo de histórico (vencedores.txt).

📋 Funcionalidades

Lê até 1000 cartelas do arquivo cartelas.txt.

Sorteia 4 cartelas para a partida.

O jogador pode alternar entre as cartelas durante o jogo.

Sorteio automático de números de 1 a 50, sem repetição.

Marca automaticamente os números sorteados nas cartelas.

Destaca visualmente a cartela vencedora.

Registra o nome do vencedor com data e hora em vencedores.txt.

📂 Estrutura de Arquivos
.
├── bingo.c          # Código-fonte principal
├── cartelas.txt     # Lista de cartelas (uma por linha, com 5 números)
└── vencedores.txt   # Histórico dos vencedores (gerado automaticamente)

🛠 Pré-requisitos

Compilador C compatível, Ex: GBD Online Compiler, VSCode (MinGW).

Arquivo cartelas.txt com pelo menos 4 cartelas válidas.

Formato do arquivo: cada linha deve conter 5 números inteiros separados por espaço ou vírgula, por exemplo:

1, 7, 15, 23, 42
5, 10, 20, 30, 40
3, 8, 22, 33, 44
6, 9, 14, 28, 50

▶ Como Compilar e Executar

Acesse o site : GBD Online Compiler ([text](https://www.onlinegdb.com/))

Escolha a Linguagem C

Copie e cole o codigo do arquivo bingo.c

Crie um novo arquivo de texte e cole o cartelas.txt

Executar o programa bingo.c

🎮 Como Jogar

Ao iniciar, o programa exibirá as cartelas sorteadas.

Pressione ENTER para sortear um número.

Digite 1, 2, 3 ou 4 para trocar de cartela controlada.

O jogo termina quando alguma cartela é completada:

Se for sua cartela: você vence e pode registrar seu nome.

Se for outra cartela: o jogo informa que outra cartela venceu.

📝 Histórico de Vencedores

Os nomes dos vencedores e as datas das vitórias são registrados em vencedores.txt, por exemplo:

2025-09-21 09:45:32 - Matheus Miranda
2025-09-22 14:20:10 - Julia Furlan

💡 Possíveis Melhorias

Permitir múltiplos jogadores simultâneos.

Configurar número de cartelas sorteadas e tamanho do bingo.

Validar automaticamente o conteúdo do arquivo cartelas.txt.