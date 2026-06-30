CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS =
TARGET = compressor
SRCS = compressor.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean run

clean:
rm -f $(OBJS) $(TARGET)

run: $(TARGET)
./$(TARGET)
