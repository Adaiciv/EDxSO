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










## Data

23/06

## Alterações

- Inseridas as estruturas -No e -heap

- Inseridas as funções de CriarNo(), contarFreq() e criarHeap().

## Justificativa

Agora estamos começando a colocar a arvore de Huffman. A árvore necessariamente precisa da frequência de bytes em um arquivo para fazer a compactação no futuro, e também precisa do heap. O heap ordena a árvore para que os bytes que menos aparece sejam a raiz, o que facilita achar os 2 menores valores para fazer o método de Huffman

## Prompt 1

"Me diga qual o próximo passo e me explique detalhadamente como funciona e como devo fazer."










## Data

26/06

## Alterações

- Inseridas as funções de inserirHeap(), removerMin() e construirHuffman

- Inserido o uso de argumentos na função main() para utilizar outros tipos de arquivos.

- Corrigido erro de escrita na função criarNo(), onde usava unsigned char freq ao invés de unsigned long freq 

## Justificativa

A função de criar o heap foi implementada anteriormente, agora, foi implementada a função de inseriri no Heap, que irá receber as frequências dos nós e irá ordená-los de forma que a raiz seja sempre o valor de menor frequencia.

A função de remover mínimo irá remover o menor valor da árvore, e quando um nó é removido, o valor que está em ultimo no vetor irá para a raiz, e, se o valor que foi para a raiz não estiver de acordo com o heap (o menor da árvore), irá procurar nas sub-arvores da esquerda e da direita o menor valor para realizar troca até estar correto. O valor removido é retornado para ser usado depois pela função de construit a árvore de Huffman

A função de construirHuffman irá usar da função de remover mínimo para pegar o menor e o segundo menor que estão no heap, irá criar um nó pai, que será a soma das duas frequências dos nós, e irá inserir o valor da frequencia no heap, enquanto o tamanho ser maior que 1. No final, retorna o ultimo elemento do heap, que é a raiz.

Implementada também a função de escolher o nome e extensão do arquivo a ser compactado e do arquivo resultante.

## Prompt 1

"Me explique como funciona o heap e a arvore de huffman em código, qual é a lógica por traz deles?"