#include <stdio.h>
/*
 * Tetris Stack - Fila de Peças Futuras
 * Programa que simula uma fila circular de peças do Tetris.
 * Permite jogar (dequeue) e inserir novas peças (enqueue).
 * Autor: Curadoria de TI (exemplo educacional)
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CAPACIDADE_FILA 5
#define CAPACIDADE_PILHA 3

// Representa uma peça com tipo (nome) e id único
typedef struct {
    char nome; // 'I', 'O', 'T', 'L'
    int id;    // identificador sequencial
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[CAPACIDADE_FILA];
    int inicio;   // índice do primeiro elemento
    int tamanho;  // número de elementos presentes
} Fila;

// Estrutura da pilha linear (reserva)
typedef struct {
    Peca itens[CAPACIDADE_PILHA];
    int topo; // índice do topo (-1 quando vazia)
} Pilha;

// ---------- Funções da fila ----------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->tamanho = 0;
}

int filaVazia(Fila *f) {
    return f->tamanho == 0;
}

int filaCheia(Fila *f) {
    return f->tamanho == CAPACIDADE_FILA;
}

int enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return 0;
    int pos = (f->inicio + f->tamanho) % CAPACIDADE_FILA;
    f->itens[pos] = p;
    f->tamanho++;
    return 1;
}

int dequeue(Fila *f, Peca *out) {
    if (filaVazia(f)) return 0;
    *out = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % CAPACIDADE_FILA;
    f->tamanho--;
    return 1;
}

// ---------- Funções da pilha ----------
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == CAPACIDADE_PILHA - 1;
}

int push(Pilha *p, Peca item) {
    if (pilhaCheia(p)) return 0;
    p->topo++;
    p->itens[p->topo] = item;
    return 1;
}

int pop(Pilha *p, Peca *out) {
    if (pilhaVazia(p)) return 0;
    *out = p->itens[p->topo];
    p->topo--;
    return 1;
}

// Gera uma peça com tipo aleatório e id sequencial
Peca gerarPeca() {
    static int contadorId = 0;
    const char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.nome = tipos[rand() % (sizeof(tipos)/sizeof(tipos[0]))];
    p.id = contadorId++;
    return p;
}

// Mostra estado atual da fila e da pilha
void mostrarEstado(Fila *f, Pilha *p) {
    printf("\nEstado atual:\n\n");

    // Mostrar fila
    printf("Fila de peças\t");
    if (filaVazia(f)) {
        printf("(vazia)");
    } else {
        for (int i = 0; i < f->tamanho; ++i) {
            int pos = (f->inicio + i) % CAPACIDADE_FILA;
            printf("[%c %d] ", f->itens[pos].nome, f->itens[pos].id);
        }
    }
    printf("\n");

    // Mostrar pilha (Topo -> Base)
    printf("Pilha de reserva \t(Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("(vazia)");
    } else {
        for (int i = p->topo; i >= 0; --i) {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
    }
    printf("\n");

    printf("Tabela: Visualização atual da fila de peças e da pilha de reserva.\nCuradoria de TI.\n\n");

    printf("Opções de Ação:\n\n");
    printf("Código\t Ação\n");
    printf("1\tJogar peça\n");
    printf("2\tReservar peça\n");
    printf("3\tUsar peça reservada\n");
    printf("0\tSair\n");
    printf("Tabela: Comandos para movimentar peças entre a fila e a pilha de reserva.\nCuradoria de TI.\n\n");
}

// Limpa resto da linha em stdin
void limparEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int main(void) {
    srand((unsigned)time(NULL));

    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa a fila com CAPACIDADE_FILA peças
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
            // Jogar peça: remove da frente da fila
            Peca jogada;
            if (!dequeue(&fila, &jogada)) {
                printf("Fila vazia. Nao ha peças para jogar.\n");
            } else {
                printf("Voce jogou a peça: [%c %d]\n", jogada.nome, jogada.id);
            }
        } else if (opcao == 2) {
            // Reservar peça: move da frente da fila para o topo da pilha
            if (pilhaCheia(&pilha)) {
                printf("Reserva cheia. Nao eh possivel reservar mais peças.\n");
            } else {
                Peca movida;
                if (!dequeue(&fila, &movida)) {
                    printf("Fila vazia. Nao ha peças para reservar.\n");
                } else {
                    if (!push(&pilha, movida)) {
                        // nao deve ocorrer pois checamos pilhaCheia, mas tratamos
                        printf("Falha ao empilhar a peca.\n");
                    } else {
                        printf("Peca reservada: [%c %d] -> topo da pilha\n", movida.nome, movida.id);
                    }
                }
            }
        } else if (opcao == 3) {
            // Usar peça reservada: pop da pilha
            Peca usada;
            if (!pop(&pilha, &usada)) {
                printf("Reserva vazia. Nao ha peças reservadas para usar.\n");
            } else {
                printf("Voce usou a peca reservada: [%c %d]\n", usada.nome, usada.id);
                // Peças usadas nao voltam para o jogo
            }
        } else if (opcao == 0) {
            printf("Saindo...\n");
            break;
        } else {
            printf("Opcao desconhecida. Use 1, 2, 3 ou 0.\n");
        }

        // Garantir que a fila esteja sempre cheia: gerar e enfileirar enquanto houver espaço
        while (!filaCheia(&fila)) {
            Peca nova = gerarPeca();
            if (!enqueue(&fila, nova)) {
                break; // segurança
            }
            printf("Nova peca gerada e adicionada: [%c %d]\n", nova.nome, nova.id);
        }
    }

    return 0;
}

