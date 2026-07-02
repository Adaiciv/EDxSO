import sys
import subprocess
import time
import csv

exe = sys.argv[1]
out_csv = sys.argv[2]
test_file = "arquivo_gigante.txt" # O arquivo de 1GB que criamos

threads_list = [1, 2, 4, 8]
results = []

for t in threads_list:
    print(f"Compactando 1GB com {t} threads... (Aguarde)")
    start = time.time()
    # Executa passando o numero de threads no final
    subprocess.run([exe, test_file, "saida_stress.czp", str(t)])
    end = time.time()
    tempo_total = end - start
    print(f" -> Tempo: {tempo_total:.2f} segundos")
    results.append((t, tempo_total))

with open(out_csv, 'w', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(["Threads", "Tempo"])
    writer.writerows(results)