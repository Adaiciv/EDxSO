#include "descompressor.h"
#include "bitstream.h"
#include "huffman.h"

int lerCabecalho(FILE *arq, Cabecalho *h){
    return fread(h, sizeof(Cabecalho), 1, arq);
}

void descompactarArquivo(FILE *origem, FILE *destino){
    Cabecalho cab;

    if(!lerCabecalho(origem,&cab)){

        printf("Erro ao ler cabecalho.\n");

        return;
    }

    if(memcmp(cab.assinatura,"HUFF",4)!=0){

        printf("Arquivo invalido.\n");

        return;
    }

    No *raiz = desserializarArvore(origem);

    No *atual = raiz;

    unsigned char buffer = 0;

    int pos = 8;

    unsigned long escritos = 0;

    if(!raiz->esq && !raiz->dir){
        for(unsigned long i = 0; i < cab.tamanhoOriginal; i++){
            fwrite(&raiz->byte, 1, 1, destino);
        }
        liberarArvore(raiz);
        return;
    }

    while(escritos < cab.tamanhoOriginal){

        int bit = lerBit(origem, &buffer, &pos);

        if(bit == -1) break;

        if(bit == 0) atual = atual->esq;
        else atual = atual->dir;

        if(!atual->esq && !atual->dir){

            fwrite(&atual->byte, 1, 1, destino);

            escritos++;

            atual = raiz;
        }
    }

    liberarArvore(raiz);
}