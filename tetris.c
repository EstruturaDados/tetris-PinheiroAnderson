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

// Estrutura da Fila Circular (Peças Futuras)
typedef struct {
    Peca itens[TAM_FILA];
    int frente;
    int tras;
    int total;
} FilaPecas;

// Estrutura da Pilha Linear (Reserva)
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

// --- GERENCIAMENTO DA FILA ---

void inicializarFila(FilaPecas *f) {
    f->frente = 0;
    f->tras = -1;
    f->total = 0;
}

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
    // Reposição automática: sempre que sai uma, entra uma nova
    enqueue(f, gerarPeca());
    return p;
}

// --- GERENCIAMENTO DA PILHA ---

void inicializarPilha(PilhaReserva *p) {
    p->topo = -1;
}

int pilhaCheia(PilhaReserva *p) {
    return p->topo == TAM_PILHA - 1;
}

int pilhaVazia(PilhaReserva *p) {
    return p->topo == -1;
}

void push(PilhaReserva *p, Peca peca) {
    if (!pilhaCheia(p)) {
        p->itens[++(p->topo)] = peca;
    } else {
        printf("\n[AVISO] Reserva cheia! Use uma peça da reserva primeiro.\n");
    }
}

Peca pop(PilhaReserva *p) {
    return p->itens[(p->topo)--];
}

// --- VISUALIZAÇÃO ---

void mostrarEstado(FilaPecas *f, PilhaReserva *p) {
    printf("\n==========================================\n");
    printf("ESTADO ATUAL\n");
    
    // Mostra Fila
    printf("Fila de pecas: ");
    for (int i = 0; i < f->total; i++) {
        int idx = (f->frente + i) % TAM_FILA;
        printf("[%c %d] ", f->itens[idx].tipo, f->itens[idx].id);
    }
    
    // Mostra Pilha
    printf("\nPilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("[Vazia]");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].tipo, p->itens[i].id);
        }
    }
    printf("\n==========================================\n");
}

int main() {
    srand(time(NULL));
    FilaPecas fila;
    PilhaReserva pilha;
    
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Populando fila inicial
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    int opcao;
    do {
        mostrarEstado(&fila, &pilha);
        printf("Opcoes de Acao:\n");
        printf("1 - Jogar peca\n");
        printf("2 - Reservar peca (Fila -> Pilha)\n");
        printf("3 - Usar peca reservada (Sair da Pilha)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                {
                    Peca p = dequeue(&fila);
                    printf("\n>>> VOCE JOGOU A PECA: [%c %d]\n", p.tipo, p.id);
                }
                break;
            case 2:
                if (!pilhaCheia(&pilha)) {
                    Peca p = dequeue(&fila); // Remove da frente da fila
                    push(&pilha, p);         // Coloca no topo da pilha
                    printf("\n>>> PECA [%c %d] ENVIADA PARA RESERVA.\n", p.tipo, p.id);
                } else {
                    printf("\n[ERRO] A reserva ja esta no limite de %d pecas!\n", TAM_PILHA);
                }
                break;
            case 3:
                if (!pilhaVazia(&pilha)) {
                    Peca p = pop(&pilha);
                    printf("\n>>> VOCE USOU A PECA RESERVADA: [%c %d]\n", p.tipo, p.id);
                } else {
                    printf("\n[ERRO] Nao ha pecas na reserva!\n");
                }
                break;
            case 0:
                printf("\nSaindo do jogo...\n");
                break;
            default:
                printf("\nOpcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}