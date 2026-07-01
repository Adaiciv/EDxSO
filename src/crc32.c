#include "crc32.h"

uint32_t calcular_crc32(const unsigned char *dados, size_t tamanho){
    uint32_t crc = 0xFFFFFFFF;

    for(size_t i = 0; i < tamanho; i++){
        crc ^= dados[i];

        for(int j = 0; j < 8; j++){
            if(crc & 1){
                crc = (crc >> 1) ^ 0xEDB88320;
            }else{
                crc >>= 1;
            }
        }
    }
    return ~crc;
}