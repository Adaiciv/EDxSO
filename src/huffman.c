#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "huffman.h"
#include "heap.h"

static int ehFolha(No *r){
    return r != NULL && r->esq == NULL && r->dir == NULL;
}

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
    if(!h) return NULL;

    while(h->tam > 1){
        No *a = removerMin(h);
        No *b = removerMin(h);

        if(!a || !b){
            return NULL;
        }

        No *pai = criarNo('*', a->freq + b->freq);
        if(!pai) return NULL;

        pai->esq = a;
        pai->dir = b;

        inserirHeap(h, pai);
    }

    return (h->tam == 1) ? removerMin(h) : NULL;
}

void gerarCodigos(No *r, char codigo[], int nivel, char *tabela[]){
    if(!r || !codigo || !tabela) return;

    if(ehFolha(r)){
        if(nivel <= 0){
            codigo[0] = '0';
            codigo[1] = '\0';
        } else {
            codigo[nivel] = '\0';
        }

        tabela[r->byte] = malloc(strlen(codigo) + 1);
        if(!tabela[r->byte]) return;

        strcpy(tabela[r->byte], codigo);
        return;
    }

    if(r->esq != NULL){
        codigo[nivel] = '0';
        gerarCodigos(r->esq, codigo, nivel + 1, tabela);
    }

    if(r->dir != NULL){
        codigo[nivel] = '1';
        gerarCodigos(r->dir, codigo, nivel + 1, tabela);
    }
}

void serializarArvore(No *r, FILE *destino){
    if(!r || !destino) return;

    if(ehFolha(r)){
        unsigned char marca = 1;
        fwrite(&marca, 1, 1, destino);
        fwrite(&r->byte, 1, 1, destino);
        return;
    }

    unsigned char marca = 0;
    fwrite(&marca, 1, 1, destino);

    serializarArvore(r->esq, destino);
    serializarArvore(r->dir, destino);
}

No *desserializarArvore(FILE *origem){
    unsigned char marca;

    if(!origem) return NULL;

    if(fread(&marca, 1, 1, origem) != 1){
        return NULL;
    }

    if(marca){
        unsigned char byte;
        if(fread(&byte, 1, 1, origem) != 1){
            return NULL;
        }
        return criarNo(byte, 0);
    }

    No *pai = criarNo('*', 0);
    if(!pai) return NULL;

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