#include "pipeline.h"
#include "crc32.h"
#include "compressor.h" 
#include <stdlib.h>
#include <pthread.h>

#define CAPACIDADE_FILA 100

typedef struct FilaSincronizada {
    BlocoArquivo *blocos[CAPACIDADE_FILA];
    int inicio;
    int fim;
    int quantidade;

    pthread_mutex_t trava;
    pthread_cond_t pode_produzir;
    pthread_cond_t pode_consumir;
} FilaSincronizada;

FilaSincronizada fila_entrada;
FilaSincronizada fila_saida;

int leitura_concluida = 0;
int total_blocos_lidos = -1;

void* thread_leitura(void* arg){
    FILE* origem = (FILE*)arg;
    int id_atual = 0;

    while(1) {
        BlocoArquivo *novo_bloco =  malloc(sizeof(BlocoArquivo));
        novo_bloco->id_bloco = id_atual;

        novo_bloco->tamanho_real = fread(novo_bloco->dados, 1, TAMANHO_BLOCO, origem);

        if(novo_bloco->tamanho_real <= 0) {
            free(novo_bloco);
            break;
        }

        novo_bloco->checksum_crc32 = calcular_crc32(novo_bloco->dados, novo_bloco->tamanho_real);

        pthread_mutex_lock(&fila_entrada.trava);

        while (fila_entrada.quantidade == CAPACIDADE_FILA) {
            pthread_cond_wait(&fila_entrada.pode_produzir, &fila_entrada.trava);
        }

        fila_entrada.blocos[fila_entrada.fim] = novo_bloco;
        fila_entrada.fim = (fila_entrada.fim + 1) % CAPACIDADE_FILA;
        fila_entrada.quantidade++;

        pthread_cond_broadcast(&fila_entrada.pode_consumir);
        pthread_mutex_unlock(&fila_entrada.trava);

        id_atual++;

    }

    pthread_mutex_lock(&fila_entrada.trava);
    total_blocos_lidos = id_atual;
    leitura_concluida = 1;
    pthread_cond_broadcast(&fila_entrada.pode_consumir);
    pthread_mutex_unlock(&fila_entrada.trava);

    return NULL;
}

void* thread_codificadora(void* arg) {

    while(1) {
        BlocoArquivo *bloco_para_processar = NULL;

        pthread_mutex_lock(&fila_entrada.trava);

        while (fila_entrada.quantidade == 0 && !leitura_concluida) {
            pthread_cond_wait(&fila_entrada.pode_consumir, &fila_entrada.trava);
        }

        if (fila_entrada.quantidade == 0 && leitura_concluida) {
            pthread_mutex_unlock(&fila_entrada.trava);
            
            // ==========================================
            // O PULO DO GATO AQUI: Acorde a escritora antes de morrer!
            pthread_mutex_lock(&fila_saida.trava);
            pthread_cond_broadcast(&fila_saida.pode_consumir);
            pthread_mutex_unlock(&fila_saida.trava);
            // ==========================================
            
            break; // Agora sim pode sair
        }

        bloco_para_processar = fila_entrada.blocos[fila_entrada.inicio];
        fila_entrada.inicio = (fila_entrada.inicio + 1) % CAPACIDADE_FILA;
        fila_entrada.quantidade--;

        pthread_cond_signal(&fila_entrada.pode_produzir);
        pthread_mutex_unlock(&fila_entrada.trava);

        compactarBloco_Para_Pipeline(bloco_para_processar, (char **)arg);

        pthread_mutex_lock(&fila_saida.trava);

        while (fila_saida.quantidade == CAPACIDADE_FILA) {
            pthread_cond_wait(&fila_saida.pode_produzir, &fila_saida.trava);
        }

        fila_saida.blocos[fila_saida.fim] = bloco_para_processar;
        fila_saida.fim = (fila_saida.fim + 1) % CAPACIDADE_FILA;
        fila_saida.quantidade++;

        pthread_cond_broadcast(&fila_saida.pode_consumir);
        pthread_mutex_unlock(&fila_saida.trava);

    }

    return NULL;
}

void* thread_escritora(void* arg){
    FILE* destino = (FILE*)arg;
    int bloco_esperado = 0;

    while(1) {
        pthread_mutex_lock(&fila_saida.trava);

        // CONDIÇÃO DE PARADA CORRETA: Leu tudo e já escreveu todos os blocos.
        if (leitura_concluida && bloco_esperado == total_blocos_lidos) {
            pthread_mutex_unlock(&fila_saida.trava);
            break;
        }

        int indice_encontrado = -1;
        for(int i = 0; i < CAPACIDADE_FILA; i++){
            if(fila_saida.blocos[i] != NULL && fila_saida.blocos[i]->id_bloco == bloco_esperado){
                indice_encontrado = i;
                break;
            }
        }

        // Se o bloco que precisamos ainda não chegou, dorme e espera!
        if(indice_encontrado == -1){
            pthread_cond_wait(&fila_saida.pode_consumir, &fila_saida.trava);
            pthread_mutex_unlock(&fila_saida.trava);
            continue; 
        }

        // Achou o bloco
        BlocoArquivo *bloco_para_escrever = fila_saida.blocos[indice_encontrado];
        fila_saida.blocos[indice_encontrado] = NULL; 
        fila_saida.quantidade--;
        pthread_cond_broadcast(&fila_saida.pode_produzir); // Acorda os codificadores
        
        pthread_mutex_unlock(&fila_saida.trava);

        // Escreve os dados no disco
        if(bloco_para_escrever){
            fwrite(&bloco_para_escrever->checksum_crc32, sizeof(uint32_t), 1, destino);
            
            // ADICIONE ESTA LINHA AQUI EMBAIXO:
            fwrite(&bloco_para_escrever->tamanho_real, sizeof(int), 1, destino); 
            
            fwrite(&bloco_para_escrever->tamanho_comprimido, sizeof(size_t), 1, destino);
            fwrite(bloco_para_escrever->dados_comprimidos, 1, bloco_para_escrever->tamanho_comprimido, destino);
            
            free(bloco_para_escrever->dados_comprimidos);
            free(bloco_para_escrever);
            
            bloco_esperado++;
        }
    }
    return NULL;
}

void iniciar_pipeline(FILE *origem, FILE *destino, int num_codificadores, void *tabela_huffman) {
    pthread_mutex_init(&fila_entrada.trava, NULL);
    pthread_cond_init(&fila_entrada.pode_produzir, NULL);
    pthread_cond_init(&fila_entrada.pode_consumir, NULL);
    
    pthread_mutex_init(&fila_saida.trava, NULL);
    pthread_cond_init(&fila_saida.pode_produzir, NULL);
    pthread_cond_init(&fila_saida.pode_consumir, NULL);

    pthread_t t_leitora;
    pthread_t t_escritora;
    pthread_t t_codificadores[num_codificadores];

    pthread_create(&t_leitora, NULL, thread_leitura, origem);
    pthread_create(&t_escritora, NULL, thread_escritora, destino);
    
    for (int i = 0; i < num_codificadores; i++) {
        pthread_create(&t_codificadores[i], NULL, thread_codificadora, tabela_huffman);
    }

    pthread_join(t_leitora, NULL);
    for (int i = 0; i < num_codificadores; i++) {
        pthread_join(t_codificadores[i], NULL);
    }
    pthread_join(t_escritora, NULL);

    pthread_mutex_destroy(&fila_entrada.trava);
    pthread_mutex_destroy(&fila_saida.trava);
    // (Opcional: destruir as condvars aqui)
}