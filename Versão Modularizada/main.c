#include <stdio.h>
#include <stdlib.h>

#include "compressor.h"
#include "descompressor.h"
#include "util.h"

int main(int argc, char *argv[]){
    if(argc != 3){
        printf("Uso: %s origem destino\n", argv[0]);
        return 1;
    }

    FILE *origem = fopen(argv[1], "rb");

    if(!origem){
        printf("Erro ao abrir o arquivo de origem.\n");
        return 1;
    }

    int compactado = ehArquivoHuff(argv[1]);

    if(!compactado){
        fseek(origem, 0, SEEK_END);

        long tamanho = ftell(origem);

        rewind(origem);

        if(tamanho == 0){
            printf("O arquivo esta vazio.\n");
            fclose(origem);
            return 1;
        }
    }

    FILE *destino = fopen(argv[2], "wb");

    if(!destino){
        printf("Erro ao criar o arquivo de destino.\n");
        fclose(origem);
        return 1;
    }

    if(compactado) descompactarArquivo(origem, destino);
    else processarArquivo(origem, destino);

    fclose(origem);
    fclose(destino);

    return 0;
}