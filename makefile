CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS =
TARGET = compressor
# Compile todos os .c da pasta exceto o descompressor (opcional)
SRCS = $(filter-out descompressor.c,$(wildcard *.c))
OBJS = $(SRCS:.c=.o)

SCRIPTS = scripts
RESULTS = results
PLOTS = plots
PY = python

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: all | $(RESULTS)
	$(PY) $(SCRIPTS)/run_tests.py ./$(TARGET) $(RESULTS)/tests.csv

stress: all | $(RESULTS)
	$(PY) $(SCRIPTS)/stress_test.py ./$(TARGET) $(RESULTS)/stress.csv

plots: | $(PLOTS) $(RESULTS)
	$(PY) $(SCRIPTS)/plot_results.py $(RESULTS) $(PLOTS)

$(RESULTS):
	mkdir -p $(RESULTS)

$(PLOTS):
	mkdir -p $(PLOTS)

clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf $(RESULTS) $(PLOTS)

.PHONY: all test stress plots clean run

run: all
	./$(TARGET)
