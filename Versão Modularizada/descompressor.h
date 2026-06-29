#ifndef DESCOMPRESSOR_H
#define DESCOMPRESSOR_H

#include "compressor.h"
#include "bitstream.h"

int lerCabecalho(FILE *arq, Cabecalho *h);

void descompactarArquivo(FILE *origem, FILE *destino);

#endif