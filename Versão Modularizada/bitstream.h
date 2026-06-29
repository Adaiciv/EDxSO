#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <stdio.h>

typedef struct{

    unsigned char buffer;

    int pos;

} BitWriter;

typedef struct{

    unsigned char buffer;

    int pos;

} BitReader;

void iniciarWriter(BitWriter *bw);

void escreverBit(FILE *destino, BitWriter *bw, int bit);

void finalizarWriter(FILE *destino, BitWriter *bw);

void iniciarReader(BitReader *br);

int lerBit(FILE *origem, BitReader *br);

#endif