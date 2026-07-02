#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heap.h"

// Nó da árvore de Huffman.
typedef struct No
{
    unsigned char byte; // símbolo/byte armazenado na folha
    unsigned long freq; // frequência/peso do nó
    struct No *esq;     // filho esquerdo
    struct No *dir;     // filho direito
} No;

// Cria um nó com byte e frequência.
No *criarNo(unsigned char byte, unsigned long freq);

// Constrói a árvore de Huffman a partir de uma heap mínima.
No *construirHuffman(heap *h);

// Gera os códigos binários para cada byte movendo-se na árvore.
// - "codigo" é um buffer usado durante a recursão.
// - "nivel" indica a profundidade atual no percurso.
// - "tabela" recebe as strings dos códigos por byte (256 entradas).
void gerarCodigos(No *r, char codigo[], int nivel, char *tabela[]);

// Serializa a árvore para o arquivo de destino (formato pre-order com marcadores).
void serializarArvore(No *r, FILE *destino);

// Desserializa a árvore do fluxo de entrada retornando a raiz.
No *desserializarArvore(FILE *origem);

// Libera toda a memória alocada pela árvore.
void liberarArvore(No *r);

#endif