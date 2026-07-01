#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "huffman.h"
#include "bitstream.h"

#define TAMANHO_BLOCO 4096

typedef struct No No;

typedef struct BlocoArquivo{
    int id_bloco;
    unsigned char dados[TAMANHO_BLOCO];
    int tamanho_real;
    uint32_t checksum_crc32;
    unsigned char *dados_comprimidos; 
    size_t tamanho_comprimido;
} BlocoArquivo;

typedef struct{
    char assinatura[4];
    unsigned long tamanhoOriginal;
}Cabecalho;

void processarArquivo(FILE *origem, FILE *destino);

unsigned long contarFreq(FILE *arq, unsigned long freq[]);

void compactarArquivo(FILE *origem, FILE *destino, char *tabela[]);

void compactarBloco_Para_Pipeline(BlocoArquivo *bloco, char *tabela[]);

void escreverCabecalho(FILE *destino, No *raiz, unsigned long tamanhoOriginal);


#endif