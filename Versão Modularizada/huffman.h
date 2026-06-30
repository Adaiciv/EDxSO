#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heap.h"

typedef struct No{
    unsigned char byte;
    unsigned long freq;
    struct No *esq;
    struct No *dir;
}No;

No *criarNo(unsigned char byte, unsigned long freq);

No *construirHuffman(heap *h);

void gerarCodigos(No *r, char codigo[], int nivel, char *tabela[]);

void serializarArvore(No *r, FILE *destino);

No *desserializarArvore(FILE *origem);

void liberarArvore(No *r);

#endif