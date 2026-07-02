import sys
import subprocess
import os
import csv

exe = sys.argv[1]
out_csv = sys.argv[2]

# Cria um arquivo de teste rápido
with open("testinho.txt", "w") as f:
    f.write("Teste basico do compressor huffman " * 1000)

subprocess.run([exe, "testinho.txt", "testinho.czp"])
subprocess.run([exe, "testinho.czp", "testinho_out.txt"])

original_size = os.path.getsize("testinho.txt")
comp_size = os.path.getsize("testinho.czp")

with open(out_csv, 'w', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(["Arquivo", "Tamanho Original", "Tamanho Comprimido"])
    writer.writerow(["testinho.txt", original_size, comp_size])

print(f"[OK] Testes basicos passaram. Original: {original_size}b -> Comprimido: {comp_size}b")