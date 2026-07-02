#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

// Verifica se o nome do arquivo possui a extensão esperada para arquivos
// compactados (.huff). Retorna 1 se for .huff, 0 caso contrário.
int ehArquivoHuff(const char *nome)
{
    const char *ext = strrchr(nome, '.');
    if (!ext)
        return 0;
    return strcmp(ext, ".czp") == 0;
}

// Imprime no console a tabela de códigos (útil para depuração).
// Mostra apenas entradas não-nulas.
void imprimirCodigos(char *tabela[])
{
    for (int i = 0; i < 256; i++)
    {
        if (tabela[i])
            printf("%c : %s\n", i, tabela[i]);
    }
}

// Libera todas as strings alocadas em "tabela" (até 256 entradas).
void liberarTabela(char *tabela[])
{
    for (int i = 0; i < 256; i++)
    {
        if (tabela[i])
            free(tabela[i]);
    }
}