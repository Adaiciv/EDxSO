#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <stdint.h>

#define TAMANHO_BLOCO 4096

typedef struct {
    int id_bloco;
    unsigned char dados[TAMANHO_BLOCO];
    int tamanho_real;
    uint32_t checksum_crc32;
} BlocoArquivo;

#endif