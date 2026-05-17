#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CAPACIDADE_FILA 5
#define CAPACIDADE_PILHA 3

// Representa uma peça com tipo e identificador único.
typedef struct {
    char nome; // 'I', 'O', 'T', 'L'
    int id;    // identificador sequencial
} Peca;

// Fila circular de peças.
typedef struct {
    Peca itens[CAPACIDADE_FILA];
    int inicio;   // índice do primeiro elemento
    int tamanho;  // número de elementos presentes
} Fila;

// Pilha de reserva de peças.
typedef struct {
    Peca itens[CAPACIDADE_PILHA];
    int topo; // índice do topo (-1 quando vazia)
} Pilha;

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->tamanho = 0;
}

int filaVazia(const Fila *f) {
    return f->tamanho == 0;
}

int filaCheia(const Fila *f) {
    return f->tamanho == CAPACIDADE_FILA;
}

int enqueue(Fila *f, Peca p) {
    if (filaCheia(f))
        return 0;

    int pos = (f->inicio + f->tamanho) % CAPACIDADE_FILA;
    f->itens[pos] = p;
    f->tamanho++;
    return 1;
}

int dequeue(Fila *f, Peca *out) {
    if (filaVazia(f))
        return 0;

    *out = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % CAPACIDADE_FILA;
    f->tamanho--;
    return 1;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(const Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(const Pilha *p) {
    return p->topo == CAPACIDADE_PILHA - 1;
}

int push(Pilha *p, Peca item) {
    if (pilhaCheia(p))
        return 0;

    p->topo++;
    p->itens[p->topo] = item;
    return 1;
}

int pop(Pilha *p, Peca *out) {
    if (pilhaVazia(p))
        return 0;

    *out = p->itens[p->topo];
    p->topo--;
    return 1;
}

Peca gerarPeca(void) {
    static int contadorId = 0;
    const char tipos[] = "IOTL";
    Peca p;
    p.nome = tipos[rand() % (sizeof(tipos) - 1)];
    p.id = contadorId++;
    return p;
}

void mostrarFila(const Fila *f) {
    if (filaVazia(f)) {
        printf("(vazia)");
        return;
    }

    for (int i = 0; i < f->tamanho; ++i) {
        int pos = (f->inicio + i) % CAPACIDADE_FILA;
        printf("[%c %d] ", f->itens[pos].nome, f->itens[pos].id);
    }
}

void mostrarPilha(const Pilha *p) {
    if (pilhaVazia(p)) {
        printf("(vazia)");
        return;
    }

    for (int i = p->topo; i >= 0; --i) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
}

void mostrarEstado(const Fila *f, const Pilha *p) {
    printf("\nEstado atual:\n\n");
    printf("Fila de peças\t");
    mostrarFila(f);
    printf("\n");

    printf("Pilha de reserva\t(Topo -> Base): ");
    mostrarPilha(p);
    printf("\n\n");

    printf("1 - Jogar peça\n");
    printf("2 - Reservar peça\n");
    printf("3 - Usar peça reservada\n");
    printf("0 - Sair\n");
    printf("\n");
}

void limparEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        ;
    }
}

int main(void) {
    srand((unsigned)time(NULL));

    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    for (int i = 0; i < CAPACIDADE_FILA; ++i) {
        enqueue(&fila, gerarPeca());
    }

    int opcao = -1;
    while (opcao != 0) {
        mostrarEstado(&fila, &pilha);
        printf("Opção: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida. Tente novamente.\n");
            limparEntrada();
            continue;
        }

        limparEntrada();

        if (opcao == 1) {
            Peca jogada;
            if (!dequeue(&fila, &jogada)) {
                printf("Fila vazia. Não há peças para jogar.\n");
            } else {
                printf("Você jogou a peça: [%c %d]\n", jogada.nome, jogada.id);
            }
        } else if (opcao == 2) {
            if (pilhaCheia(&pilha)) {
                printf("Reserva cheia. Não é possível reservar mais peças.\n");
            } else {
                Peca movida;
                if (!dequeue(&fila, &movida)) {
                    printf("Fila vazia. Não há peças para reservar.\n");
                } else if (!push(&pilha, movida)) {
                    printf("Falha ao mover peça para a reserva.\n");
                } else {
                    printf("Peça reservada: [%c %d] -> topo da pilha\n", movida.nome, movida.id);
                }
            }
        } else if (opcao == 3) {
            Peca usada;
            if (!pop(&pilha, &usada)) {
                printf("Reserva vazia. Não há peças reservadas para usar.\n");
            } else {
                printf("Você usou a peça reservada: [%c %d]\n", usada.nome, usada.id);
            }
        } else if (opcao == 0) {
            printf("Saindo...\n");
            break;
        } else {
            printf("Opção desconhecida. Use 1, 2, 3 ou 0.\n");
        }

        while (!filaCheia(&fila)) {
            Peca nova = gerarPeca();
            enqueue(&fila, nova);
            printf("Nova peça gerada e adicionada: [%c %d]\n", nova.nome, nova.id);
        }
    }

    return 0;
}

