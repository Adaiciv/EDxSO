#ifndef CRC32_H
#define CRC32_H

#include <stddef.h>
#include <stdint.h>

uint32_t calcular_crc32(const unsigned char *dados, size_t tamanho);

#endif