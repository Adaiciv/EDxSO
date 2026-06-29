#include "compressor.h"
#include "huffman.h"
#include "heap.h"
#include "bitstream.h"
#include "util.h"

void processarArquivo(FILE *origem, FILE *destino){
    unsigned long freq[256] = {0};

    unsigned long tamanhoOriginal = contarFreq(origem, freq);

    heap *h = criarHeap(256);

    for(int i = 0; i < 256; i++){
        if(freq[i] > 0) inserirHeap(h, criarNo(i, freq[i]));
    };

    if(h->tam==0){

        printf("Arquivo vazio.\n");

        liberarHeap(h);

        return;
    }

    No *raiz = construirHuffman(h);

    char caminho[256];

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

unsigned long contarFreq(FILE *arq, unsigned long freq[]){
    unsigned char c;

    unsigned long tamanho = 0;

    while(fread(&c, 1, 1, arq) == 1){
        freq[c]++;
        tamanho++;
    }

    return tamanho;
}

void compactarArquivo(FILE *origem, FILE *destino, char *tabela[]){
    unsigned char c;

    unsigned char buffer = 0;

    int pos = 0;

    while(fread(&c, 1, 1, origem) == 1){
        
        char *codigo = tabela[c];

        if(!codigo) return;

        for(int i = 0; codigo[i] != '\0'; i++){
            escreverBit(destino, codigo[i] == '1', &buffer, &pos);
        }
    }

    if(pos > 0) fwrite(&buffer, 1, 1, destino);
}

void escreverCabecalho(FILE *destino, No *raiz, unsigned long tamanhoOriginal){
    Cabecalho h;

    memcpy(h.assinatura,"HUFF",4);

    h.tamanhoOriginal = tamanhoOriginal;

    fwrite(&h, sizeof(Cabecalho), 1, destino);

    serializarArvore(raiz,destino);
}