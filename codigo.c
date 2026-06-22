#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *origem = fopen("entrada.txt", "rb");
    FILE *destino = fopen("saida.txt", "wb");

    if (!origem || !destino) {
        printf("Erro ao abrir os arquivos.\n");

        if (origem) fclose(origem);
        if (destino) fclose(destino);

        return 1;
    }

    char buffer[1024];
    size_t lidos;

    while ((lidos = fread(buffer, 1, sizeof(buffer), origem)) > 0) {
        fwrite(buffer, 1, lidos, destino);
    }

    fclose(origem);
    fclose(destino);

    printf("Arquivo copiado com sucesso.\n");

    return 0;
}