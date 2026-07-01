# 1. Configurações rigorosas do Edital
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -pthread -O2
TSAN_FLAGS = -fsanitize=thread -g

# 2. Caminho correto da pasta src
SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

# 3. Nome exigido pelo projeto
TARGET = czip

# Pastas e scripts do Python
SCRIPTS = scripts
RESULTS = results
PLOTS = plots
PY = python3

# ==========================================
# ALVOS PRINCIPAIS
# ==========================================

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# ==========================================
# TESTES E AUTOMAÇÃO (A parte de vocês corrigida)
# ==========================================

test: all | $(RESULTS)
	@echo "--- Rodando testes em Python ---"
	$(PY) $(SCRIPTS)/run_tests.py ./$(TARGET) $(RESULTS)/tests.csv

stress: all | $(RESULTS)
	@echo "--- Rodando teste de estresse em Python ---"
	$(PY) $(SCRIPTS)/stress_test.py ./$(TARGET) $(RESULTS)/stress.csv

plots: | $(PLOTS) $(RESULTS)
	@echo "--- Gerando os gráficos ---"
	$(PY) $(SCRIPTS)/plot_results.py $(RESULTS) $(PLOTS)

$(RESULTS):
	mkdir -p $(RESULTS)

$(PLOTS):
	mkdir -p $(PLOTS)

clean:
	rm -f $(SRC_DIR)/*.o $(TARGET) czip_tsan arquivo_gigante.bin *.czp
	rm -rf $(RESULTS) $(PLOTS)

.PHONY: all test stress plots clean run

run: all
	./$(TARGET)