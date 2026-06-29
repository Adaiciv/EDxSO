#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ehArquivoHuff(const char *nome);

void imprimirCodigos(char *tabela[]);

void liberarTabela(char *tabela[]);

#endif