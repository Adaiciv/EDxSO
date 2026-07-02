#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <stdio.h>

typedef struct
{

    unsigned char buffer;

    int pos;

} BitWriter;

typedef struct
{

    unsigned char buffer;

    int pos;

} BitReader;

// Inicializa o estado do "BitWriter" antes de começar a escrever bits.
void iniciarWriter(BitWriter *bw);

// Escreve um bit (0 ou 1) no arquivo de saída usando o "BitWriter".
// Os bits são acumulados em um byte interno e gravados quando completos.
void escreverBit(FILE *destino, BitWriter *bw, int bit);

// Finaliza a escrita: grava o último byte parcial (se houver) no arquivo.
void finalizarWriter(FILE *destino, BitWriter *bw);

// Inicializa o "BitReader" para começar a ler bits de um fluxo.
void iniciarReader(BitReader *br);

// Lê um bit do arquivo (0 ou 1). Retorna -1 em caso de EOF/erro.
int lerBit(FILE *origem, BitReader *br);

#endif