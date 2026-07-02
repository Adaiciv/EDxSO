#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "huffman.h"
#include "bitstream.h"

#define TAMANHO_BLOCO 4096

typedef struct No No;

typedef struct BlocoArquivo
{
    int id_bloco;
    unsigned char dados[TAMANHO_BLOCO];
    int tamanho_real;
    uint32_t checksum_crc32;
    unsigned char *dados_comprimidos;
    size_t tamanho_comprimido;
} BlocoArquivo;

// Estrutura "BlocoArquivo" usada para o pipeline concorrente:
// - "id_bloco": identificador sequencial do bloco (ordenação).
// - "dados": buffer com dados lidos do arquivo (até "TAMANHO_BLOCO").
// - "tamanho_real": número de bytes válidos em "dados" lidos do arquivo.
// - "checksum_crc32": CRC32 do bloco original para verificação de integridade.
// - "dados_comprimidos": ponteiro para dados após compressão (alocado dinamicamente).
// - "tamanho_comprimido": tamanho em bytes de "dados_comprimidos".
typedef struct
{
    char assinatura[4];
    unsigned long tamanhoOriginal;
} Cabecalho;

// Processa um arquivo completo: monta árvore Huffman, escreve cabeçalho e compacta.
void processarArquivo(FILE *origem, FILE *destino);

// Conta a frequência de bytes no arquivo "arq". Retorna o tamanho total lido.
unsigned long contarFreq(FILE *arq, unsigned long freq[]);

// Compacta todo o arquivo usando a tabela de códigos (fluxo tradicional).
void compactarArquivo(FILE *origem, FILE *destino, char *tabela[]);

// Compacta um único "BlocoArquivo" usando a tabela Huffman para o pipeline:
// - Preenche "dados_comprimidos" (alocado) e "tamanho_comprimido".
void compactarBloco_Para_Pipeline(BlocoArquivo *bloco, char *tabela[]);

// Escreve o cabeçalho (assinatura + tamanho original) e serializa a árvore
// de Huffman no arquivo de destino.
void escreverCabecalho(FILE *destino, No *raiz, unsigned long tamanhoOriginal);

#endif