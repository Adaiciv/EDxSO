#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Verifica se o nome do arquivo tem extensão de arquivo compactado (.huff).
// Retorna 1 se for .huff, 0 caso contrário.
int ehArquivoHuff(const char *nome);

// Imprime a tabela de códigos Huffman no console (apenas entradas não-nulas).
// Útil para depuração.
void imprimirCodigos(char *tabela[]);

// Libera as strings alocadas dentro da tabela de códigos (até 256 entradas).
void liberarTabela(char *tabela[]);

#endif