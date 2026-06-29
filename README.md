# EDxSO

## Codigo normal:

 - Compilação: 
        gcc codigo.c -o codigo.exe

 - Execução (compactação):
        ./codigo.exe teste1.txt teste1.huff

 - Execução (Descompactação):
        ./codigo.exe teste1.huff resultado.txt
    
## Codigo Modularizado:

 - Compilação:
        gcc main.c heap.c huffman.c compressor.c descompressor.c bitstream.c util.c -o huff
 - Execução (Compactação):
        ./huff teste1.txt teste1.huff
 - Execução (Descompactação):
        ./huff teste1.huff resultado.txt
