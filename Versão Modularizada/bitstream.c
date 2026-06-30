#include "bitstream.h"

void iniciarWriter(BitWriter *bw)
{
    bw->buffer = 0;
    bw->pos = 0;
}

void finalizarWriter(FILE *destino, BitWriter *bw){
    if(bw->pos > 0){

        fwrite(&bw->buffer, 1, 1, destino);

        bw->buffer = 0;
        bw->pos = 0;
    }
}

void escreverBit(FILE *destino, BitWriter *bw, int bit){
    if(bit) bw->buffer |= (1 << (7 - bw->pos));

    bw->pos++;

    if(bw->pos == 8){

        fwrite(&bw->buffer, 1, 1, destino);

        bw->buffer = 0;
        bw->pos = 0;
    }
}

void iniciarReader(BitReader *br){
    br->buffer = 0;
    br->pos = 8;
}

int lerBit(FILE *origem, BitReader *br){
    if(br->pos == 8){

        if(fread(&br->buffer, 1, 1, origem) != 1) return -1;

        br->pos = 0;
    }

    int bit = (br->buffer >> (7 - br->pos)) & 1;

    br->pos++;

    return bit;
}