#ifndef BISTREAM_H
#define BITSTREAM_H

#include "huffman.h"

int lerBit(FILE *origem, unsigned char *buffer, int *pos);

void escreverBit(FILE *destino, int bit, unsigned char *buffer, int *pos);

#endif