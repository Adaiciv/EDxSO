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









## Data

27/06

## Alterações

- Inserida a estrutura do cabeçalho.

- Alteradas a função de contarFreq().

- inseridas as funções de gerarCódigos(), escreverCabecalho(), escreverBit(), compactarArquivo(), lerCabecalho(), imprimirCodigos e funções de liberação de memória.

- Inseridas alterações no main

## Justificativa

Após a construção da árvore de Huffman, tornou-se necessário armazenar informações que permitam identificar o arquivo compactado e reconstruir a árvore durante a descompactação. Para isso, foi criada uma estrutura de cabeçalho contendo uma assinatura para identificar o formato do arquivo, o tamanho original do arquivo e o vetor de frequências utilizado para gerar a árvore.

A função contarFreq() passou a retornar também o tamanho original do arquivo, além de preencher o vetor de frequências. Essa informação será gravada no cabeçalho e permitirá que a descompactação saiba exatamente quantos bytes devem ser reconstruídos, evitando interpretar bits de preenchimento presentes no último byte compactado.

Inserida a função gerarCodigos() pois, após a construção da árvore de Huffman, foi necessário percorrê-la para gerar os códigos binários de cada byte. A função realiza um percurso recursivo na árvore, registrando '0' ao seguir para a esquerda e '1' ao seguir para a direita, armazenando o código correspondente de cada símbolo em uma tabela para ser utilizada na compactação.

A função escreverCabecalho() foi criada para gravar, no início do arquivo compactado, todas as informações necessárias para sua futura descompactação. Dessa forma, o arquivo passa a ser autocontido, contendo os dados necessários para reconstruir a árvore de Huffman sem depender do arquivo original.

A função escreverBit() é necessária pois, como os códigos de Huffman possuem tamanho variável e são formados por bits, tornou-se necessário criar uma função responsável por agrupar esses bits em um buffer de 8 bits (1 byte). Quando o buffer é preenchido, seu conteúdo é gravado no arquivo compactado, permitindo o armazenamento eficiente dos códigos.

A função compactarArquivo() é responsável por realizar a compactação propriamente dita. A função percorre novamente o arquivo original, consulta o código de Huffman correspondente a cada byte na tabela gerada anteriormente e utiliza escreverBit() para gravar esses códigos em formato binário no arquivo de saída

A função lerCabecalho() Foi adicionada para preparar a etapa de descompactação. Sua função é ler as informações presentes no cabeçalho do arquivo compactado, permitindo recuperar os dados necessários para reconstruir a árvore de Huffman e restaurar o conteúdo original.

A função imprimirCodigos() foi criada para auxiliar na validação do algoritmo durante os testes. A função imprime os códigos de Huffman gerados para cada byte presente no arquivo, permitindo verificar se a árvore foi construída corretamente e se os códigos atribuídos estão consistentes.

Foram implementadas funções responsáveis por liberar a memória alocada dinamicamente para a árvore de Huffman, para a tabela de códigos e para o heap. Essa etapa evita vazamentos de memória e garante que todos os recursos utilizados pelo programa sejam corretamente liberados ao final da execução.

A função main() foi reorganizada para representar todas as etapas do processo de compactação utilizando o algoritmo de Huffman. Inicialmente, passou a receber os nomes dos arquivos de origem e destino por meio dos argumentos da linha de comando (argc e argv), tornando o programa mais flexível e permitindo sua utilização com qualquer arquivo, sem a necessidade de alterar o código-fonte.

Também foi adicionada a verificação da quantidade de argumentos informados e da abertura dos arquivos, garantindo que o programa seja encerrado de forma segura caso ocorra algum erro durante a execução.

Após a abertura dos arquivos, a função passou a executar a sequência completa do algoritmo: contagem das frequências dos bytes, criação do heap mínimo, construção da árvore de Huffman, geração dos códigos binários e escrita do cabeçalho do arquivo compactado.

Foi inserida a chamada da função rewind() antes da compactação para reposicionar o ponteiro do arquivo de origem no início. Isso é necessário porque a função de contagem de frequências percorre todo o arquivo, deixando o ponteiro no final. Sem esse reposicionamento, a etapa de compactação não conseguiria realizar uma nova leitura dos dados.

Ao final da execução, foram adicionadas chamadas para liberar toda a memória alocada dinamicamente (heap, árvore de Huffman e tabela de códigos) e fechar corretamente os arquivos utilizados, evitando vazamentos de memória e garantindo o encerramento adequado do programa.

## Prompt 1

"Agora quais seriam os próximos passos? me explique."

## Prompt 2
"Como faço para que o código exiba as especificações do arquivo compactado, como tamanho?"

## Prompt 3
"Como eu deveria fazer a função main?"