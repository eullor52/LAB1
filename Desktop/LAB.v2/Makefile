CC = gcc
CFLAGS = -Wall -Wextra -Iheaders

SRCDIR = srcs
OBJDIR_PROG = obj_prog
OBJDIR_TEST = obj_test

ALL_SRCS = $(wildcard $(SRCDIR)/*.c)

PROG_SRCS = $(filter-out $(SRCDIR)/test.c, $(ALL_SRCS))
PROG_OBJS = $(addprefix $(OBJDIR_PROG)/, $(notdir $(PROG_SRCS:.c=.o)))

TEST_SRCS = $(filter-out $(SRCDIR)/main.c, $(ALL_SRCS))
TEST_OBJS = $(addprefix $(OBJDIR_TEST)/, $(notdir $(TEST_SRCS:.c=.o)))

.PHONY: all clean fclean re program tests

all: program

program: $(PROG_OBJS)
	$(CC) $^ -o $@

tests: $(TEST_OBJS)
	$(CC) $^ -o $@

$(OBJDIR_PROG)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR_PROG)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR_TEST)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR_TEST)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR_PROG) $(OBJDIR_TEST)

fclean: clean
	rm -f program.exe tests.exe

re: fclean all