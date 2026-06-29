#include <stdio.h>
#include <string.h>

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

    if(memcmp(cab.assinatura, "HUFF", 4)!=0){

        printf("Arquivo invalido.\n");

        return;
    }

    No *raiz = desserializarArvore(origem);

    if(!raiz){

        printf("Erro ao reconstruir a arvore.\n");

        return;
    }

    if(!raiz->esq && !raiz->dir){

        for(unsigned long i=0; i<cab.tamanhoOriginal; i++){
            fwrite(&raiz->byte, 1, 1, destino);
        }

        liberarArvore(raiz);

        return;
    }

    BitReader br;

    iniciarReader(&br);

    unsigned long escritos = 0;

    No *atual = raiz;

    while(escritos < cab.tamanhoOriginal){

        int bit = lerBit(origem, &br);

        if(bit == -1) break;

        if(bit == 0)
            atual = atual->esq;
        else
            atual = atual->dir;

        if(!atual->esq && !atual->dir){

            fwrite(&atual->byte, 1, 1, destino);

            escritos++;

            atual = raiz;
        }
    }

    liberarArvore(raiz);
}