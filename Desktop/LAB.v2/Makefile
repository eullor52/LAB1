CC = gcc
CFLAGS = -Wall -Wextra -Iheaders

SRCDIR = srcs
OBJS_DIR = objs
LIBRARY = liblineform.lib
TARGET = program.exe
TESTS = tests.exe
MAIN = main.c
TEST_SRC = test.c

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJS_DIR)/%.o,$(SOURCES))

lib: $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	ar rcs $@ $^

$(OBJS_DIR):
	mkdir -p $@

$(OBJS_DIR)/%.o: $(SRCDIR)/%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

program: $(TARGET)
$(TARGET): $(MAIN) $(LIBRARY)
	$(CC) $^ -o $@

tests: $(TESTS)
$(TESTS): $(TEST_SRC) $(LIBRARY)
	$(CC) $^ -o $@

run: program
	./$(TARGET)

run_tests: $(TESTS)
	./$(TESTS)

all: program tests

clean:
	rm -f $(LIBRARY) $(TARGET) $(TESTS)
	rm -rf $(OBJS_DIR)

.PHONY: lib program tests all clean run run_tests