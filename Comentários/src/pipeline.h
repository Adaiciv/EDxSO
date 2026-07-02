#ifndef PIPELINE_H
#define PIPELINE_H

#include "compressor.h"
#include <stdio.h>
#include <pthread.h>

// Inicia o pipeline concorrente de leitura -> compressão -> escrita.
// - "origem": arquivo de entrada aberto em modo binário (leitura).
// - "destino": arquivo de saída aberto em modo binário (escrita).
// - "num_threads_codificadoras": número de threads que irão compactar blocos em paralelo.
// - "tabela_huffman": ponteiro para a tabela de códigos gerada (passada para as codificadoras).
//
// O pipeline sincroniza produtores/consumidores usando filas protegidas por mutex
// e variáveis de condição. A função cria as threads internas e bloqueia até
*que todo o trabalho esteja concluído.
        * /
    void iniciar_pipeline(FILE *origem, FILE *destino, int num_threads_codificadoras, void *tabela_huffman);

#endif
