#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <stdio.h>
#include "huffman.h"
#include "bitstream.h"

typedef struct No No;

typedef struct{
    char assinatura[4];
    unsigned long tamanhoOriginal;
}Cabecalho;

void processarArquivo(FILE *origem, FILE *destino);

unsigned long contarFreq(FILE *arq, unsigned long freq[]);

void compactarArquivo(FILE *origem, FILE *destino, char *tabela[]);

void compactarBloco(unsigned char *dados, size_t tamanho, FILE *destino, char *tabela[], BitWriter *bw);

void escreverCabecalho(FILE *destino, No *raiz, unsigned long tamanhoOriginal);


#endif