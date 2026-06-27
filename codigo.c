#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No{
    unsigned char byte;
    unsigned long freq;
    struct No *esq;
    struct No *dir;
}No;

typedef struct{
    No **v;
    int tam;
    int capacidade;
}heap;

typedef struct{
    char assinatura[4];
    unsigned long tamanhoOriginal;
    unsigned long freq;
}Cabecalho;

unsigned long contarFreq(FILE *arq, unsigned long freq[]){
    unsigned char c;

    unsigned long tamanho = 0;

    while(fread(&c, 1, 1, arq) == 1){
        freq[c]++;
        tamanho++;
    }

    return tamanho;
}

No *criarNo(unsigned char byte, unsigned long freq){
    No *novo = (No *)malloc(sizeof(No));

    novo->byte = byte;
    novo->freq = freq;
    novo->esq = NULL;
    novo->dir = NULL;

    return novo;
}

heap *criarHeap(int capacidade){
    heap *h = malloc(sizeof(heap));

    h->v = malloc(sizeof(No*) * capacidade);

    h->tam = 0;

    h->capacidade = capacidade;

    return h;
}

//heap *h = criarHeap(256)

void inserirHeap(heap *h, No *novo){
    int i = h->tam;

    h->v[i] = novo;

    h->tam++;

    while(i > 0){

        int pai = (i - 1) / 2;

        if(h->v[pai]->freq <= h->v[i]->freq) break;

        No *aux = h->v[pai];
        h->v[pai] = h->v[i];
        h->v[i] = aux;

        i = pai;
    }
}

No *removerMin(heap *h){
    if(h->tam == 0) return NULL;

    No *menor = h->v[0];

    h->tam--;

    h->v[0] = h->v[h->tam];

    int i = 0;

    while(1){
        int esq = 2*i + 1;
        int dir = 2*i + 2;
        int menorFilho = i;

        if(esq < h->tam && h->v[esq]->freq < h->v[menorFilho]->freq)
            menorFilho = esq;
        if (dir < h->tam && h->v[dir]->freq < h->v[menorFilho]->freq)
            menorFilho = dir;
        if(menorFilho == i) break;

        No *aux = h->v[i];
        h->v[i] = h->v[menorFilho];
        h->v[menorFilho] = aux;

        i = menorFilho;
    }

    return menor;
}

No *construirHuffman(heap *h){
    while(h->tam > 1){
        No *a = removerMin(h);
        No *b = removerMin(h);

        No *pai = criarNo('*', a->freq + b->freq);

        pai->esq = a;
        pai->dir = b;

        inserirHeap(h, pai);
    }

    return removerMin(h);
}

void gerarCodigos(No *r, char codigo[], int nivel, char *tabela[]){
    if(!r) return;

    if(!r->esq && !r->dir){
        codigo[nivel] = '\0';

        tabela[r->byte] = malloc(strlen(codigo) + 1);

        strcpy(tabela[r->byte], codigo);

        return;
    }

    codigo[nivel] = '0';
    gerarCodigos(r->esq, codigo, nivel + 1, tabela);

    codigo[nivel] = '1';
    gerarCodigos(r->dir, codigo, nivel + 1, tabela);
}

void escreverCabecalho(FILE *destino, unsigned long freq[], unsigned long tamanhoOriginal){
    Cabecalho h;

    memcpy(h.assinatura, "HUFF", 4);

    h.tamanhoOriginal = tamanhoOriginal;

    memcpy(h.freq, freq, sizeof(unsigned long) * 256);

    fwrite(&h, sizeof(Cabecalho), 1, destino);
}

void escreverBit(FILE *destino, int bit, unsigned char *buffer, int *pos){
    if(bit)
        *buffer |= (1 << (7 - *pos));
    (*pos)++;

    if(*pos == 8){
        fwrite(buffer, 1, 1, destino);

        *buffer = 0;

        *pos = 0;
    }
    
}

void compactarArquivo(FILE *origem, FILE *destino, char *tabela[]){
    unsigned char c;

    unsigned char buffer = 0;

    int pos = 0;

    while(fread(&c, 1, 1, origem) == 1){
        
        char *codigo = tabela[c];

        for(int i = 0; codigo[i] != '\0'; i++){
            escreverBit(destino, codigo[i] == '1', &buffer, &pos);
        }
    }

    if(pos > 0) fwrite(&buffer, 1, 1, destino);
}

int lerCabecalho(FILE *arq, Cabecalho *h){
    return fread(h, sizeof(Cabecalho), 1, arq);
}

void imprimirCodigos(char *tabela[]){
    for(int i = 0; i < 256; i++){
        if(tabela[i])
            printf("%c : %s\n", i, tabela[i]);
    }
}

void liberarArvore(No *r){
    if(!r) return;

    liberarArvore(r->esq);

    liberarArvore(r->dir);

    free(r);
}

void liberarTabela(char *tabela[]){
    for(int i = 0; i < 256; i++){
        if(tabela[i]) free(tabela[i]);
    }
}

void liberarHeap(heap *h){
    free(h->v);
    free(h);
}

//argc - argument count - numero total de argumentos passados
//argv - argument vector - array de ponteiros para strings, onde cada elemento aponta para um argumento especifico.
int main(int argc, char *argv[]){

    if(argc != 3){
        printf("Uso: %s origem destino\n", argv[0]);
        return 1;
    }

    FILE *origem = fopen(argv[1], "rb");
    FILE *destino = fopen(argv[2], "wb");

    if (!origem || !destino) {
        printf("Erro ao abrir os arquivos.\n");

        if (origem) fclose(origem);
        if (destino) fclose(destino);

        return 1;
    }

    unsigned long freq[256] = {0};

    unsigned long tamanhoOriginal = contarFreq(origem, freq);

    heap *h = criarHeap(256);

    for(int i = 0; i < 256; i++){
        if(freq[i] > 0){
            inserirHeap(h, criarNo(i, freq[i]));
        }
    };

    if(h->tam==0){

        printf("Arquivo vazio.\n");

        liberarHeap(h);

        fclose(origem);

        fclose(destino);

        return 1;
    }

    No *raiz = construirHuffman(h);

    char caminho[256];

    char *tabela[256]={NULL};

    gerarCodigos(raiz, caminho, 0, tabela);

    imprimirCodigos(tabela);

    escreverCabecalho(destino, freq, tamanhoOriginal);

    rewind(origem);

    compactarArquivo(origem, destino, tabela);

    long tamanhoCompactado = ftell(destino);

    printf("\nArquivo original : %lu bytes\n", tamanhoOriginal);

    printf("Arquivo compactado: %ld bytes\n", tamanhoCompactado);

    printf("Taxa de compressao: %.2f%%\n", 100.0 * (1.0 - (double)tamanhoCompactado / tamanhoOriginal));
    

    liberarTabela(tabela);

    liberarArvore(raiz);

    liberarHeap(h);

    fclose(origem);

    fclose(destino);


    return 0;
}