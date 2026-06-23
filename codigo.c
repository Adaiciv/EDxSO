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

No *criarNo(unsigned char byte, unsigned char freq){
    No *novo = (No *)malloc(sizeof(No));

    novo->byte = byte;
    novo->freq = freq;
    novo->esq = NULL;
    novo->dir = NULL;

    return novo;
}

heap *criarHeap(int capacidade){
    heap *h = malloc(sizeof(heap));

    h->v = malloc(sizeof(No) * capacidade);

    h->tam = 0;

    h->capacidade = capacidade;

    return h;
}

//heap *h = criarHeap(256)

int main() {
    FILE *origem = fopen("entrada.txt", "rb");
    FILE *destino = fopen("saida.txt", "wb");

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