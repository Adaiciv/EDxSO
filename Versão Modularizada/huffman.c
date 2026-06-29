#include <stdlib.h>
#include <string.h>

#include "huffman.h"
#include "heap.h"

No *criarNo(unsigned char byte, unsigned long freq){
    No *novo = (No *)malloc(sizeof(No));
    if(!novo) return NULL;

    novo->byte = byte;
    novo->freq = freq;
    novo->esq = NULL;
    novo->dir = NULL;

    return novo;
}

No *construirHuffman(heap *h){
    while(h->tam > 1){
        No *a = removerMin(h);
        No *b = removerMin(h);

        No *pai = criarNo('*', a->freq + b->freq);
        if(!pai) return NULL;

        pai->esq = a;
        pai->dir = b;

        inserirHeap(h, pai);
    }

    return removerMin(h);
}

void gerarCodigos(No *r, char codigo[], int nivel, char *tabela[]){
    if(!r) return;

    if(!r->esq && !r->dir){   
        if(nivel == 0){
            codigo[0] = '0';
            codigo[1] = '\0';
        }
        else{
            codigo[nivel] = '\0';
        }

        tabela[r->byte] = malloc(strlen(codigo) + 1);
        if(!tabela[r->byte]) return;

        strcpy(tabela[r->byte], codigo);

        return;
    }

    codigo[nivel] = '0';
    gerarCodigos(r->esq, codigo, nivel + 1, tabela);

    codigo[nivel] = '1';
    gerarCodigos(r->dir, codigo, nivel + 1, tabela);
}

void serializarArvore(No *r, FILE *destino){
    if(!r) return;

    if(!r->esq && !r->dir){

        unsigned char marca = 1;

        fwrite(&marca,1,1,destino);

        fwrite(&r->byte,1,1,destino);

        return;
    }

    unsigned char marca = 0;

    fwrite(&marca,1,1,destino);

    serializarArvore(r->esq,destino);

    serializarArvore(r->dir,destino);
}

No *desserializarArvore(FILE *origem){
    unsigned char marca;

    if(fread(&marca,1,1,origem)!=1) return NULL;

    if(marca){

        unsigned char byte;

        fread(&byte,1,1,origem);

        return criarNo(byte,0);
    }

    No *pai = criarNo('*',0);

    pai->esq = desserializarArvore(origem);

    pai->dir = desserializarArvore(origem);

    return pai;
}

void liberarArvore(No *r){
    if(!r) return;

    liberarArvore(r->esq);

    liberarArvore(r->dir);

    free(r);
}