#include "huffman.h"

int lerBit(FILE *origem, unsigned char *buffer, int *pos){
    if(*pos == 8){

        if(fread(buffer,1,1,origem) != 1) return -1;
        *pos = 0;
    }

    int bit = (*buffer >> (7 - *pos)) & 1;

    (*pos)++;

    return bit;
}

void escreverBit(FILE *destino, int bit, unsigned char *buffer, int *pos){
    if(bit)
        *buffer |= (1 << (7 - *pos));
    (*pos)++;

    if(*pos == 8){
        fwrite(buffer, 1, 1, destino);

        *buffer = 0;

        *pos = 0;
    }
    
}