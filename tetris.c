#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

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

// Estrutura da Pilha Linear
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} PilhaReserva;

int proximoId = 0;

// --- FUNÇÕES DE APOIO ---

Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.tipo = tipos[rand() % 4];
    nova.id = proximoId++;
    return nova;
}

void inicializarEstruturas(FilaPecas *f, PilhaReserva *p) {
    f->frente = 0;
    f->tras = -1;
    f->total = 0;
    p->topo = -1;
}

// --- OPERAÇÕES DE FILA ---

void enqueue(FilaPecas *f, Peca p) {
    if (f->total < TAM_FILA) {
        f->tras = (f->tras + 1) % TAM_FILA;
        f->itens[f->tras] = p;
        f->total++;
    }
}

Peca dequeue(FilaPecas *f) {
    Peca p = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->total--;
    enqueue(f, gerarPeca()); // Reposição automática
    return p;
}

// --- OPERAÇÕES DE PILHA ---

void push(PilhaReserva *p, Peca peca) {
    p->itens[++(p->topo)] = peca;
}

Peca pop(PilhaReserva *p) {
    return p->itens[(p->topo)--];
}

// --- OPERAÇÕES MESTRE (TROCAS) ---

// Troca a peça da frente da fila com o topo da pilha
void trocarPecaAtual(FilaPecas *f, PilhaReserva *p) {
    if (f->total > 0 && p->topo != -1) {
        Peca temp = f->itens[f->frente];
        f->itens[f->frente] = p->itens[p->topo];
        p->itens[p->topo] = temp;
        printf("\n[OK] Troca simples realizada com sucesso!\n");
    } else {
        printf("\n[ERRO] Acao impossivel: fila ou pilha vazia.\n");
    }
}

// Alterna os 3 primeiros da fila com os 3 da pilha
void trocaMultipla(FilaPecas *f, PilhaReserva *p) {
    if (f->total >= 3 && p->topo == 2) { // Pilha cheia (id 0, 1, 2)
        for (int i = 0; i < 3; i++) {
            int idxFila = (f->frente + i) % TAM_FILA;
            int idxPilha = p->topo - i; // Topo para base
            
            Peca temp = f->itens[idxFila];
            f->itens[idxFila] = p->itens[idxPilha];
            p->itens[idxPilha] = temp;
        }
        printf("\n[OK] Troca multipla de 3 pecas realizada!\n");
    } else {
        printf("\n[ERRO] Requisitos: Fila e Pilha devem ter ao menos 3 pecas.\n");
    }
}

// --- INTERFACE ---

void mostrarEstado(FilaPecas *f, PilhaReserva *p) {
    printf("\n--------------------------------------------------");
    printf("\nFila de pecas: ");
    for (int i = 0; i < f->total; i++) {
        int idx = (f->frente + i) % TAM_FILA;
        printf("[%c %d] ", f->itens[idx].tipo, f->itens[idx].id);
    }
    printf("\nPilha reserva (Topo->Base): ");
    if (p->topo == -1) printf("[Vazia]");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].tipo, p->itens[i].id);
    }
    printf("\n--------------------------------------------------\n");
}

int main() {
    srand(time(NULL));
    FilaPecas fila;
    PilhaReserva pilha;
    inicializarEstruturas(&fila, &pilha);

    for (int i = 0; i < TAM_FILA; i++) enqueue(&fila, gerarPeca());

    int opcao;
    do {
        mostrarEstado(&fila, &pilha);
        printf("1. Jogar peca da frente\n2. Reservar peca (Fila -> Pilha)\n");
        printf("3. Usar peca reservada\n4. Trocar frente da fila com topo da pilha\n");
        printf("5. Trocar 3 primeiros da fila com 3 da pilha\n0. Sair\nEscolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: printf("\nJogou: [%c %d]\n", dequeue(&fila).tipo, (proximoId-TAM_FILA-1)); break;
            case 2: 
                if (pilha.topo < TAM_PILHA - 1) push(&pilha, dequeue(&fila));
                else printf("\nPilha cheia!\n");
                break;
            case 3:
                if (pilha.topo != -1) printf("\nUsou da reserva: [%c %d]\n", pop(&pilha).tipo, 0);
                else printf("\nPilha vazia!\n");
                break;
            case 4: trocarPecaAtual(&fila, &pilha); break;
            case 5: trocaMultipla(&fila, &pilha); break;
        }
    } while (opcao != 0);
    return 0;
}