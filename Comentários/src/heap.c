#include "heap.h"
#include "huffman.h"

// Troca dois ponteiros para nós no vetor da heap.
static void trocar(No **a, No **b)
{
    No *aux = *a;
    *a = *b;
    *b = aux;
}

// Aloca e inicializa uma heap com capacidade inicial "capacidade".
// Retorna NULL se a alocação falhar.
heap *criarHeap(int capacidade)
{
    heap *h = malloc(sizeof(heap));

    if (!h)
        return NULL;

    h->v = malloc(sizeof(No *) * capacidade);

    if (!h->v)
    {
        free(h);
        return NULL;
    }

    h->tam = 0;
    h->capacidade = capacidade;

    return h;
}

// Insere um novo nó na heap, expandindo o vetor se necessário,
// e realiza o ajuste (heapify-up) para manter a propriedade de min-heap.
void inserirHeap(heap *h, No *novo)
{

    if (!h || !novo)
        return;

    if (h->tam == h->capacidade)
    {
        h->capacidade *= 2;
        No **novoVetor = realloc(h->v, sizeof(No *) * h->capacidade);
        if (!novoVetor)
            return;
        h->v = novoVetor;
    }

    int i = h->tam;
    h->v[i] = novo;
    h->tam++;

    while (i > 0)
    {
        int pai = (i - 1) / 2;
        if (h->v[pai]->freq <= h->v[i]->freq)
            break;
        trocar(&h->v[pai], &h->v[i]);
        i = pai;
    }
}

// Remove e retorna o menor nó (raiz) da heap.
// Realiza heapify-down para restaurar a propriedade de min-heap.
No *removerMin(heap *h)
{
    if (h->tam == 0)
        return NULL;

    No *menor = h->v[0];
    h->tam--;
    h->v[0] = h->v[h->tam];

    int i = 0;
    while (1)
    {
        int esq = 2 * i + 1;
        int dir = 2 * i + 2;
        int menorFilho = i;

        if (esq < h->tam && h->v[esq]->freq < h->v[menorFilho]->freq)
            menorFilho = esq;
        if (dir < h->tam && h->v[dir]->freq < h->v[menorFilho]->freq)
            menorFilho = dir;
        if (menorFilho == i)
            break;

        trocar(&h->v[i], &h->v[menorFilho]);
        i = menorFilho;
    }

    return menor;
}

// Libera a memória alocada pela heap.
void liberarHeap(heap *h)
{
    if (!h)
        return;
    free(h->v);
    free(h);
}