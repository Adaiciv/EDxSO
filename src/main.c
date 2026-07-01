#include <stdio.h>
#include <stdlib.h>
#include "compressor.h"
#include "descompressor.h"
#include "util.h"
#include "pipeline.h" // Adicionado para acessar o seu Pipeline

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

    // Lógica de arquivo vazio mantida
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

    if(compactado) {
        descompactarArquivo(origem, destino);
    } else {
        // ========================================================
        // AQUI ESTÁ A INTEGRAÇÃO FINAL
        // ========================================================
        
        // 1. Pedro: Monta a estrutura da árvore (Sequencial)
        unsigned long freq[256] = {0};
        contarFreq(origem, freq); // Conta frequências
        
        heap *h = criarHeap(256);
        for(int i = 0; i < 256; i++) if(freq[i] > 0) inserirHeap(h, criarNo(i, freq[i]));
        
        No *raiz = construirHuffman(h);
        char *tabela[256] = {NULL};
        char caminho[256] = {0};
        gerarCodigos(raiz, caminho, 0, tabela);

        // 2. Escreve o cabeçalho no arquivo de destino
        escreverCabecalho(destino, raiz, 0); // O tamanho original pode ser ajustado depois se precisar

        // 3. Diêgo: Liga a Fábrica (Pipeline Concorrente)
        rewind(origem); // Volta ao início para o Leitor começar
        iniciar_pipeline(origem, destino, 4, tabela); // 4 é o número de threads codificadoras

        // 4. Limpeza
        liberarTabela(tabela);
        liberarArvore(raiz);
        liberarHeap(h);
    }

    fclose(origem);
    fclose(destino);
    return 0;
}