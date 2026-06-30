#ifndef DESCOMPRESSOR_H
#define DESCOMPRESSOR_H

#include <stdio.h>

#include "compressor.h"

int lerCabecalho(FILE *arq, Cabecalho *h);

void descompactarArquivo(FILE *origem, FILE *destino);

#endif