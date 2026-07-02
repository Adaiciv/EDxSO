#ifndef DESCOMPRESSOR_H
#define DESCOMPRESSOR_H

#include <stdio.h>

#include "compressor.h"

// Lê o cabeçalho do arquivo compactado e preenche a estrutura "Cabecalho".
int lerCabecalho(FILE *arq, Cabecalho *h);

// Descompacta o arquivo "origem" para "destino" usando a árvore serializada.
// Realiza verificação de CRC por bloco e ignora blocos corrompidos.
void descompactarArquivo(FILE *origem, FILE *destino);

#endif