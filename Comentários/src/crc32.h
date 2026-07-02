#ifndef CRC32_H
#define CRC32_H

#include <stddef.h>
#include <stdint.h>

// Calcula o CRC32 de um buffer de bytes.
// - "dados": ponteiro para os bytes.
// - "tamanho": número de bytes.
// Retorna o checksum CRC32.
uint32_t calcular_crc32(const unsigned char *dados, size_t tamanho);

#endif