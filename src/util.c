#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

int ehArquivoHuff(const char *nome){
    const char *ext = strrchr(nome, '.');

    if(!ext)
        return 0;

    return strcmp(ext, ".czp") == 0;
}

void imprimirCodigos(char *tabela[]){
    for(int i = 0; i < 256; i++){
        if(tabela[i])
            printf("%c : %s\n", i, tabela[i]);
    }
}

void liberarTabela(char *tabela[]){
    for(int i = 0; i < 256; i++){
        if(tabela[i]) free(tabela[i]);
    }
}