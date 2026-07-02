#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "descompressor.h"
#include "bitstream.h"
#include "huffman.h"
#include "crc32.h" 

int lerCabecalho(FILE *arq, Cabecalho *h){
    return fread(h, sizeof(Cabecalho), 1, arq);
}

void descompactarArquivo(FILE *origem, FILE *destino){
    Cabecalho cab;

    if(!lerCabecalho(origem, &cab)){
        printf("Erro ao ler cabecalho.\n");
        return;
    }

    if(memcmp(cab.assinatura, "HUFF", 4) != 0){
        printf("Arquivo invalido.\n");
        return;
    }

    // 1. Reconstrói a árvore a partir do arquivo
    No *raiz = desserializarArvore(origem);
    if(!raiz){
        printf("Erro ao reconstruir a arvore.\n");
        return;
    }

    uint32_t checksum_lido;
    int tamanho_real_bloco;
    size_t tamanho_comprimido;

    // 2. Loop para ler Bloco a Bloco até o arquivo acabar
    while(fread(&checksum_lido, sizeof(uint32_t), 1, origem) == 1){
        
        // Lê os metadados do bloco checando se deu certo (Resolve o erro do -Werror)
        if (fread(&tamanho_real_bloco, sizeof(int), 1, origem) != 1) break;
        if (fread(&tamanho_comprimido, sizeof(size_t), 1, origem) != 1) break;

        // Lê os bits compactados para a memória
        unsigned char *dados_comp = malloc(tamanho_comprimido);
        if (fread(dados_comp, 1, tamanho_comprimido, origem) != tamanho_comprimido) {
            free(dados_comp);
            break; // Arquivo cortado/corrompido no meio
        }

        // Prepara um buffer para os bytes que vamos descompactar
        unsigned char *dados_descomp = malloc(tamanho_real_bloco);

        int bit_pos = 0;
        int byte_pos = 0;
        int bytes_decodificados = 0;
        No *atual = raiz;

        // 3. Navega na Árvore de Huffman bit a bit
        while(bytes_decodificados < tamanho_real_bloco){
            int bit = (dados_comp[byte_pos] >> (7 - bit_pos)) & 1;
            
            bit_pos++;
            if(bit_pos == 8){
                bit_pos = 0;
                byte_pos++;
            }

            if(bit == 0) atual = atual->esq;
            else atual = atual->dir;

            // Se chegou numa folha, achou a letra original!
            if(!atual->esq && !atual->dir){
                dados_descomp[bytes_decodificados++] = atual->byte;
                atual = raiz; 
            }
        }

        // -------------------------------------------------------------
        // TESTE DE FOGO DO EDITAL: VERIFICAR A INTEGRIDADE (CRC32)
        // -------------------------------------------------------------
        uint32_t crc_calculado = calcular_crc32(dados_descomp, tamanho_real_bloco);

        if(crc_calculado == checksum_lido){
            // Bloco íntegro: Grava no arquivo final
            fwrite(dados_descomp, 1, tamanho_real_bloco, destino);
        } else {
            // Bloco corrompido: Avisa mas NÃO PARA a descompressão
            printf("AVISO: Bloco corrompido detectado! Ignorando bloco e continuando...\n");
        }

        free(dados_comp);
        free(dados_descomp);
    }

    liberarArvore(raiz);
    printf("Descompactacao concluida com sucesso!\n");
}