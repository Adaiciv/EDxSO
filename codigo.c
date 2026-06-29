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
}Cabecalho;

int ehArquivoHuff(const char *nome){
    const char *ext = strrchr(nome, '.');

    if(!ext)
        return 0;

    return strcmp(ext, ".huff") == 0;
}

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
    if(!novo) return NULL;

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
        if(nivel == 0){
            codigo[0] = '0';
            codigo[1] = '\0';
        }
        else{
            codigo[nivel] = '\0';
        }

        tabela[r->byte] = malloc(strlen(codigo) + 1);

        strcpy(tabela[r->byte], codigo);

        return;
    }

    codigo[nivel] = '0';
    gerarCodigos(r->esq, codigo, nivel + 1, tabela);

    codigo[nivel] = '1';
    gerarCodigos(r->dir, codigo, nivel + 1, tabela);
}

void serializarArvore(No *r, FILE *destino){
    if(!r) return;

    if(!r->esq && !r->dir){

        unsigned char marca = 1;

        fwrite(&marca,1,1,destino);

        fwrite(&r->byte,1,1,destino);

        return;
    }

    unsigned char marca = 0;

    fwrite(&marca,1,1,destino);

    serializarArvore(r->esq,destino);

    serializarArvore(r->dir,destino);
}

No *desserializarArvore(FILE *origem){
    unsigned char marca;

    if(fread(&marca,1,1,origem)!=1) return NULL;

    if(marca == 1){

        unsigned char byte;

        fread(&byte,1,1,origem);

        return criarNo(byte,0);
    }

    No *pai = criarNo('*',0);

    pai->esq = desserializarArvore(origem);

    pai->dir = desserializarArvore(origem);

    return pai;
}

int lerBit(FILE *origem, unsigned char *buffer, int *pos){
    if(*pos == 8){

        if(fread(buffer,1,1,origem) != 1) return -1;
        *pos = 0;
    }

    int bit = (*buffer >> (7 - *pos)) & 1;

    (*pos)++;

    return bit;
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

void escreverCabecalho(FILE *destino, No *raiz, unsigned long tamanhoOriginal){
    Cabecalho h;

    memcpy(h.assinatura,"HUFF",4);

    h.tamanhoOriginal = tamanhoOriginal;

    fwrite(&h, sizeof(Cabecalho), 1, destino);

    serializarArvore(raiz,destino);
}

int lerCabecalho(FILE *arq, Cabecalho *h){
    return fread(h, sizeof(Cabecalho), 1, arq);
}

void compactarArquivo(FILE *origem, FILE *destino, char *tabela[]){
    unsigned char c;

    unsigned char buffer = 0;

    int pos = 0;

    while(fread(&c, 1, 1, origem) == 1){
        
        char *codigo = tabela[c];

        if(!codigo) return;

        for(int i = 0; codigo[i] != '\0'; i++){
            escreverBit(destino, codigo[i] == '1', &buffer, &pos);
        }
    }

    if(pos > 0) fwrite(&buffer, 1, 1, destino);
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

void processarArquivo(FILE *origem, FILE *destino){
    unsigned long freq[256] = {0};

    unsigned long tamanhoOriginal = contarFreq(origem, freq);

    heap *h = criarHeap(256);

    for(int i = 0; i < 256; i++){
        if(freq[i] > 0) inserirHeap(h, criarNo(i, freq[i]));
    };

    if(h->tam==0){

        printf("Arquivo vazio.\n");

        liberarHeap(h);

        return;
    }

    No *raiz = construirHuffman(h);

    char caminho[256];

    char *tabela[256]={NULL};

    gerarCodigos(raiz, caminho, 0, tabela);

    imprimirCodigos(tabela);

    escreverCabecalho(destino, raiz, tamanhoOriginal);

    rewind(origem);

    compactarArquivo(origem, destino, tabela);

    long tamanhoCompactado = ftell(destino);

    printf("\nArquivo original : %lu bytes\n", tamanhoOriginal);

    printf("Arquivo compactado: %ld bytes\n", tamanhoCompactado);

    printf("Taxa de compressao: %.2f%%\n", 100.0 * (1.0 - (double)tamanhoCompactado / tamanhoOriginal));
    

    liberarTabela(tabela);

    liberarArvore(raiz);

    liberarHeap(h);
}

void descompactarArquivo(FILE *origem, FILE *destino){
    Cabecalho cab;

    if(!lerCabecalho(origem,&cab)){

        printf("Erro ao ler cabecalho.\n");

        return;
    }

    if(memcmp(cab.assinatura,"HUFF",4)!=0){

        printf("Arquivo invalido.\n");

        return;
    }

    No *raiz = desserializarArvore(origem);

    No *atual = raiz;

    unsigned char buffer = 0;

    int pos = 8;

    unsigned long escritos = 0;

    if(!raiz->esq && !raiz->dir){
        for(unsigned long i = 0; i < cab.tamanhoOriginal; i++){
            fwrite(&raiz->byte, 1, 1, destino);
        }
        liberarArvore(raiz);
        return;
    }

    while(escritos < cab.tamanhoOriginal){

        int bit = lerBit(origem, &buffer, &pos);

        if(bit == -1) break;

        if(bit == 0) atual = atual->esq;
        else atual = atual->dir;

        if(!atual->esq && !atual->dir){

            fwrite(&atual->byte, 1, 1, destino);

            escritos++;

            atual = raiz;
        }
    }

    liberarArvore(raiz);
}


//argc - argument count - numero total de argumentos passados
//argv - argument vector - array de ponteiros para strings, onde cada elemento aponta para um argumento especifico.
int main(int argc, char *argv[]){
    if(argc != 3){
        printf("Uso: %s origem destino\n", argv[0]);
        return 1;
    }

    FILE *origem = fopen(argv[1], "rb");

    if(!origem){
        printf("Erro ao abrir o arquivo de origem.\n");
        return 1;
    }

    int compactado = ehArquivoHuff(argv[1]);

    if(!compactado){
        fseek(origem, 0, SEEK_END);

        long tamanho = ftell(origem);

        rewind(origem);

        if(tamanho == 0){
            printf("O arquivo esta vazio.\n");
            fclose(origem);
            return 1;
        }
    }

    FILE *destino = fopen(argv[2], "wb");

    if(!destino){
        printf("Erro ao criar o arquivo de destino.\n");
        fclose(origem);
        return 1;
    }

    if(compactado) descompactarArquivo(origem, destino);
    else processarArquivo(origem, destino);

    fclose(origem);
    fclose(destino);

    return 0;
}