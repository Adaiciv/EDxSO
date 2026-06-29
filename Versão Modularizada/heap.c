#include "heap.h"
#include "huffman.h"

heap *criarHeap(int capacidade){
    heap *h = malloc(sizeof(heap));

    h->v = malloc(sizeof(No*) * capacidade);

    h->tam = 0;

    h->capacidade = capacidade;

    return h;
}

void inserirHeap(heap *h, No *novo){
    int i = h->tam;

    h->v[i] = novo;

    h->tam++;

    while(i > 0){

        int pai = (i - 1) / 2;

        if(h->v[pai]->freq <= h->v[i]->freq) break;

        No *aux = h->v[pai];
        h->v[pai] = h->v[i];
        h->v[i] = aux;

        i = pai;
    }
}

No *removerMin(heap *h){
    if(h->tam == 0) return NULL;

    No *menor = h->v[0];

    h->tam--;

    h->v[0] = h->v[h->tam];

    int i = 0;

    while(1){
        int esq = 2*i + 1;
        int dir = 2*i + 2;
        int menorFilho = i;

        if(esq < h->tam && h->v[esq]->freq < h->v[menorFilho]->freq)
            menorFilho = esq;
        if (dir < h->tam && h->v[dir]->freq < h->v[menorFilho]->freq)
            menorFilho = dir;
        if(menorFilho == i) break;

        No *aux = h->v[i];
        h->v[i] = h->v[menorFilho];
        h->v[menorFilho] = aux;

        i = menorFilho;
    }

    return menor;
}

void liberarHeap(heap *h){
    free(h->v);
    free(h);
}