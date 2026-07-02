#ifndef HEAP_H
#define HEAP_H

typedef struct No No;

/* Estrutura que representa uma heap mínima (min-heap) de ponteiros para `No`. */
typedef struct
{
    No **v;         /* vetor dinâmico de ponteiros para nós */
    int tam;        /* quantidade atual de elementos */
    int capacidade; /* capacidade alocada do vetor */
} heap;

/* Cria uma heap com capacidade inicial informada. */
heap *criarHeap(int capacidade);

/* Insere um nó na heap (mantém propriedade de min-heap). */
void inserirHeap(heap *h, No *novo);

/* Remove e retorna o nó com menor chave (frequência). */
No *removerMin(heap *h);

/* Libera a memória usada pela heap. */
void liberarHeap(heap *h);

#endif