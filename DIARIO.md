# DIARIO.md

## Data

21/06

## Atividade

Implementação da primeira versão da camada de leitura e escrita de arquivos do projeto de compressor de arquivos.

## Objetivo

Compreender como realizar a abertura, leitura e gravação de arquivos em C utilizando operações de baixo nível baseadas em blocos. Esta funcionalidade servirá como fundação para as próximas etapas do compressor, nas quais os dados lidos serão processados pela codificação de Huffman antes de serem gravados.

## Código Desenvolvido

Foi implementado um programa capaz de:

* Abrir um arquivo de entrada em modo binário.
* Criar ou sobrescrever um arquivo de saída.
* Ler o arquivo de entrada em blocos de 1024 bytes.
* Escrever cada bloco lido no arquivo de saída.
* Tratar erros de abertura de arquivos.
* Fechar corretamente os arquivos ao final da execução.

## Relação com o Projeto

Embora o programa ainda não realize compressão, ele implementa a infraestrutura básica necessária para o compressor.

Fluxo atual:

Arquivo → Leitura em blocos → Buffer → Escrita em blocos → Novo arquivo

Fluxo esperado futuramente:

Arquivo → Leitura em blocos → Huffman → Compressão → Escrita do arquivo compactado

## Conceitos Estudados

### fopen

Utilizado para abrir arquivos em modo leitura ("rb") e escrita ("wb").

### fread

Responsável por ler blocos de bytes do arquivo de entrada para um buffer temporário.

### fwrite

Responsável por gravar os bytes presentes no buffer no arquivo de saída.

### Buffer

Vetor de 1024 bytes utilizado como área temporária de armazenamento durante a cópia dos dados.

### Loop de Processamento

O programa continua lendo e gravando enquanto a função fread retornar uma quantidade de bytes maior que zero.

## Uso de Inteligência Artificial

Ferramenta utilizada: ChatGPT

### Prompt 1

"Me explique como eu poderia fazer o arquivo em C que consiga copiar o conteúdo de um arquivo de texto para outro arquivo de texto, e como o código poderia se tornar um compressor de arquivos utilizando a árvore de Huffman"


### Prompt 2

"Me explique como a árvore de Huffman funciona e qual a diferença dela de uma árvore AVL e BST convencional"

## Conclusão

A etapa permitiu compreender os mecanismos básicos de manipulação de arquivos em C e estabeleceu a fundação necessária para a implementação futura do compressor baseado em Árvore de Huffman.
