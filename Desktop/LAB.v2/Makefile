CC = gcc
CFLAGS = -Wall -Wextra

TARGET = program.exe
SRCS = main.c complex.c io.c operations.c lineform.c
OBJS = main.o complex.o io.o operations.o lineform.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c $< $^

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean