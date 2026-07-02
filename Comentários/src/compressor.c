#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compressor.h"
#include "heap.h"
#include "huffman.h"
#include "bitstream.h"
#include "util.h"

// Conta a frequência de cada byte no arquivo "arq".
// Lê em blocos de "TAMANHO_BLOCO" para eficiência.
// Retorna o tamanho total lido em bytes.
unsigned long contarFreq(FILE *arq, unsigned long freq[])
{
    unsigned char buffer[TAMANHO_BLOCO];
    unsigned long tamanho = 0;
    size_t lidos;

    while ((lidos = fread(buffer, 1, TAMANHO_BLOCO, arq)) > 0)
    {
        tamanho += lidos;
        for (size_t i = 0; i < lidos; i++)
            freq[buffer[i]]++;
    }

    return tamanho;
}

// Compacta um "BlocoArquivo" usando a "tabela" de códigos Huffman.
// - Calcula o número total de bits do bloco compactado.
// - Aloca "dados_comprimidos" com o tamanho em bytes adequado e escreve os bits
//   resultantes consecutivamente.
void compactarBloco_Para_Pipeline(BlocoArquivo *bloco, char *tabela[])
{
    size_t total_bits = 0;
    for (int i = 0; i < bloco->tamanho_real; i++)
    {
        char *codigo = tabela[bloco->dados[i]];
        if (codigo)
            total_bits += strlen(codigo);
    }

    size_t total_bytes = (total_bits + 7) / 8;
    bloco->dados_comprimidos = (unsigned char *)calloc(total_bytes, 1);
    bloco->tamanho_comprimido = total_bytes;

    int bit_pos = 0;
    int byte_pos = 0;
    for (int i = 0; i < bloco->tamanho_real; i++)
    {
        char *codigo = tabela[bloco->dados[i]];
        if (!codigo)
            continue;
        for (int j = 0; codigo[j] != '\0'; j++)
        {
            if (codigo[j] == '1')
            {
                bloco->dados_comprimidos[byte_pos] |= (1 << (7 - bit_pos));
            }
            bit_pos++;
            if (bit_pos == 8)
            {
                bit_pos = 0;
                byte_pos++;
            }
        }
    }
}

// Escreve o cabeçalho do arquivo compactado contendo a assinatura e o
// tamanho original, seguido pela árvore de Huffman serializada.
void escreverCabecalho(FILE *destino, No *raiz, unsigned long tamanhoOriginal)
{
    Cabecalho h;
    memcpy(h.assinatura, "HUFF", 4);
    h.tamanhoOriginal = tamanhoOriginal;
    fwrite(&h, sizeof(Cabecalho), 1, destino);
    serializarArvore(raiz, destino);
}