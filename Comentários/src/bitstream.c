#include "bitstream.h"

// Inicializa o escritor de bits.
// Define o buffer interno como 0 e a posição atual como 0 (primeiro bit).
void iniciarWriter(BitWriter *bw)
{
    bw->buffer = 0;
    bw->pos = 0;
}

// Finaliza a escrita de bits no arquivo.
// Se houver um byte parcial no buffer (pos > 0), grava-o no arquivo.
void finalizarWriter(FILE *destino, BitWriter *bw)
{
    if (bw->pos > 0)
    {
        fwrite(&bw->buffer, 1, 1, destino);

        bw->buffer = 0;
        bw->pos = 0;
    }
}

//* Escreve um único bit no fluxo de saída.
// - O bit é colocado na posição corrente do buffer (do mais significativo para o menos).
// - Quando 8 bits são preenchidos, o byte é escrito no arquivo e o buffer é reiniciado.
void escreverBit(FILE *destino, BitWriter *bw, int bit)
{
    if (bit)
        bw->buffer |= (1 << (7 - bw->pos));

    bw->pos++;

    if (bw->pos == 8)
    {
        fwrite(&bw->buffer, 1, 1, destino);

        bw->buffer = 0;
        bw->pos = 0;
    }
}

// Inicializa o leitor de bits.
// Define "pos = 8" para forçar leitura de um novo byte no primeiro "lerBit".
void iniciarReader(BitReader *br)
{
    br->buffer = 0;
    br->pos = 8;
}

// Lê um único bit do fluxo de entrada.
// - Se o buffer atual já foi consumido (pos == 8), lê um novo byte do arquivo.
// - Retorna 0 ou 1 para o bit lido, ou -1 em caso de EOF/erro de leitura.
int lerBit(FILE *origem, BitReader *br)
{
    if (br->pos == 8)
    {
        if (fread(&br->buffer, 1, 1, origem) != 1)
            return -1;
        br->pos = 0;
    }

    int bit = (br->buffer >> (7 - br->pos)) & 1;
    br->pos++;

    return bit;
}