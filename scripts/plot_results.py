import sys
import pandas as pd
import matplotlib.pyplot as plt

results_dir = sys.argv[1]
plots_dir = sys.argv[2]

df = pd.read_csv(f"{results_dir}/stress.csv")

plt.figure(figsize=(8, 5))
plt.plot(df['Threads'], df['Tempo'], marker='o', linestyle='-', color='b')
plt.title('Tempo de Compressão de 1GB vs Número de Threads')
plt.xlabel('Número de Threads')
plt.ylabel('Tempo (segundos)')
plt.grid(True)
plt.savefig(f"{plots_dir}/speedup.png")
print("[OK] Grafico salvo em plots/speedup.png")