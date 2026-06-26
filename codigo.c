#include <stdio.h>
#include <stdlib.h>

typedef struct No{
    unsigned char byte;
    unsigned long freq;
    struct No *esq;
    struct No *dir;
}No;

typedef struct{
    No **v;
    int tam;
    int capacidade;
}heap;

void contarFreq(FILE *arq, unsigned long freq[]){
    unsigned char c;

    while(fread(&c, 1, 1, arq) == 1){
        freq[c]++;
    }
}

No *criarNo(unsigned char byte, unsigned long freq){
    No *novo = (No *)malloc(sizeof(No));

    novo->byte = byte;
    novo->freq = freq;
    novo->esq = NULL;
    novo->dir = NULL;

    return novo;
}

heap *criarHeap(int capacidade){
    heap *h = malloc(sizeof(heap));

    h->v = malloc(sizeof(No*) * capacidade);

    h->tam = 0;

    h->capacidade = capacidade;

    return h;
}

//heap *h = criarHeap(256)

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

No *construirHuffman(heap *h){
    while(h->tam > 1){
        No *a = removerMin(h);
        No *b = removerMin(h);

        No *pai = criarNo('*', a->freq + b->freq);

        pai->esq = a;
        pai->dir = b;

        inserirHeap(h, pai);
    }

    return removerMin(h);
}

//argc - argument count - numero total de argumentos passados
//argv - argument vector - array de ponteiros para strings, onde cada elemento aponta para um argumento especifico.
int main(int argc, char *argv[]){

    if(argc != 3){
        printf("Uso: %s origem destino\n", argv[0]);
        return 1;
    }
    FILE *origem = fopen(argv[1], "rb");
    FILE *destino = fopen(argv[2], "wb");

    if (!origem || !destino) {
        printf("Erro ao abrir os arquivos.\n");

        if (origem) fclose(origem);
        if (destino) fclose(destino);

        return 1;
    }

    char buffer[1024];
    size_t lidos;

    while ((lidos = fread(buffer, 1, sizeof(buffer), origem)) > 0) {
        fwrite(buffer, 1, lidos, destino);
    }

    fclose(origem);
    fclose(destino);

    printf("Arquivo copiado com sucesso.\n");

    return 0;
}