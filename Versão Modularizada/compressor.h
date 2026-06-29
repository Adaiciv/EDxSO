#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "huffman.h"

typedef struct{
    char assinatura[4];
    unsigned long tamanhoOriginal;
}Cabecalho;

void processarArquivo(FILE *origem, FILE *destino);

unsigned long contarFreq(FILE *arq, unsigned long freq[]);

void compactarArquivo(FILE *origem, FILE *destino, char *tabela[]);

void escreverCabecalho(FILE *destino, No *raiz, unsigned long tamanhoOriginal);


#endif