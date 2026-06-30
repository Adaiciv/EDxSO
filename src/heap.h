#ifndef HEAP_H
#define HEAP_H

typedef struct No No;

typedef struct{
    No **v;
    int tam;
    int capacidade;
}heap;

heap *criarHeap(int capacidade);

void inserirHeap(heap *h, No *novo);

No *removerMin(heap *h);

void liberarHeap(heap *h);

#endif