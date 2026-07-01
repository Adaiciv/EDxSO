#ifndef PIPELINE_H
#define PIPELINE_H

#include "compressor.h"
#include <stdio.h>
#include <pthread.h>

void iniciar_pipeline(FILE *origem, FILE *destino, int num_threads_codificadoras, void *tabela_huffman);

#endif
