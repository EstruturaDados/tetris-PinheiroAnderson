#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5

// Estrutura que representa uma peça do jogo
typedef struct {
    char tipo;
    int id;
} Peca;

// Estrutura da Fila Circular
typedef struct {
    Peca itens[TAM_FILA];
    int frente;
    int tras;
    int total;
} FilaPecas;

// Variável global para controle de IDs únicos
int proximoId = 0;

// Função para gerar uma peça aleatória
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.tipo = tipos[rand() % 4];
    nova.id = proximoId++;
    return nova;
}

// Inicializa a fila definindo os índices
void inicializarFila(FilaPecas *f) {
    f->frente = 0;
    f->tras = -1;
    f->total = 0;
}

int filaCheia(FilaPecas *f) {
    return f->total == TAM_FILA;
}

int filaVazia(FilaPecas *f) {
    return f->total == 0;
}

// Insere uma peça no final (lógica circular)
void enqueue(FilaPecas *f, Peca p) {
    if (!filaCheia(f)) {
        f->tras = (f->tras + 1) % TAM_FILA;
        f->itens[f->tras] = p;
        f->total++;
    } else {
        printf("\nErro: A fila de peças está cheia!\n");
    }
}

// Remove a peça da frente (lógica circular)
Peca dequeue(FilaPecas *f) {
    Peca p = {' ', -1};
    if (!filaVazia(f)) {
        p = f->itens[f->frente];
        f->frente = (f->frente + 1) % TAM_FILA;
        f->total--;
    } else {
        printf("\nErro: Nenhuma peça na fila!\n");
    }
    return p;
}

// Exibe o estado visual da fila
void mostrarFila(FilaPecas *f) {
    printf("\nFila de peças: ");
    for (int i = 0; i < f->total; i++) {
        int indice = (f->frente + i) % TAM_FILA;
        printf("[%c %d] ", f->itens[indice].tipo, f->itens[indice].id);
    }
    printf("\n");
}

int main() {
    srand(time(NULL)); // Semente para números aleatórios
    FilaPecas fila;
    inicializarFila(&fila);

    // Inicializa a fila com 5 peças conforme requisito
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    int opcao;
    do {
        mostrarFila(&fila);
        printf("\nOpções de ação:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                if (!filaVazia(&fila)) {
                    Peca jogada = dequeue(&fila);
                    printf("\n>>> PEÇA JOGADA: [%c %d]\n", jogada.tipo, jogada.id);
                } else {
                    printf("\nFila vazia!\n");
                }
                break;
            case 2:
                if (!filaCheia(&fila)) {
                    enqueue(&fila, gerarPeca());
                    printf("\nNova peça adicionada à fila.\n");
                } else {
                    printf("\nFila cheia! Jogue uma peça antes de adicionar nova.\n");
                }
                break;
            case 0:
                printf("\nEncerrando Tetris Stack...\n");
                break;
            default:
                printf("\nOpção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}