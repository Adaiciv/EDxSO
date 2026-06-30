#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compressor.h"
#include "heap.h"
#include "huffman.h"
#include "bitstream.h"
#include "util.h"

#define TAM_BLOCO 4096

unsigned long contarFreq(FILE *arq, unsigned long freq[]){
    unsigned char buffer[TAM_BLOCO];

    unsigned long tamanho = 0;

    size_t lidos;

    while((lidos = fread(buffer,1,TAM_BLOCO,arq)) > 0){

        tamanho += lidos;

        for(size_t i=0;i<lidos;i++) freq[buffer[i]]++;
    }

    return tamanho;
}

void compactarBloco(unsigned char *dados, size_t tamanho, FILE *destino, char *tabela[], BitWriter *bw){
    for(size_t j=0;j<tamanho;j++){

        char *codigo = tabela[dados[j]];

        if(!codigo) continue;

        for(int i=0;codigo[i];i++){

            escreverBit(destino, bw, codigo[i]=='1');
        }
    }
}

void compactarArquivo(FILE *origem, FILE *destino, char *tabela[]){
    unsigned char buffer[TAM_BLOCO];

    size_t lidos;

    BitWriter bw;

    iniciarWriter(&bw);

    while((lidos=fread(buffer, 1, TAM_BLOCO, origem))>0){
        compactarBloco(buffer, lidos, destino, tabela, &bw);
    }

    finalizarWriter(destino, &bw);
}

void escreverCabecalho(FILE *destino, No *raiz, unsigned long tamanhoOriginal)
{
    Cabecalho h;

    memcpy(h.assinatura,"HUFF",4);

    h.tamanhoOriginal = tamanhoOriginal;

    fwrite(&h, sizeof(Cabecalho), 1, destino);

    serializarArvore(raiz, destino);
}

void processarArquivo(FILE *origem, FILE *destino){
    unsigned long freq[256]={0};

    unsigned long tamanhoOriginal =
        contarFreq(origem, freq);

    heap *h = criarHeap(256);

    if(!h){

        printf("Erro ao criar heap.\n");

        return;
    }

    for(int i=0;i<256;i++){

        if(freq[i]>0){
            inserirHeap(h, criarNo(i, freq[i]));
        }
    }

    if(h->tam==0){

        printf("Arquivo vazio.\n");

        liberarHeap(h);

        return;
    }

    No *raiz = construirHuffman(h);

    char caminho[256]={0};

    char *tabela[256]={NULL};

    gerarCodigos(raiz, caminho, 0, tabela);

    imprimirCodigos(tabela);

    escreverCabecalho(destino, raiz, tamanhoOriginal);

    rewind(origem);

    compactarArquivo(origem, destino, tabela);

    long tamanhoCompactado = ftell(destino);

    printf("\nArquivo original : %lu bytes\n", tamanhoOriginal);

    printf("Arquivo compactado: %ld bytes\n", tamanhoCompactado);

    printf("Taxa de compressao: %.2f%%\n", 100.0 * (1.0 - (double)tamanhoCompactado / tamanhoOriginal));

    liberarTabela(tabela);

    liberarArvore(raiz);

    liberarHeap(h);
}