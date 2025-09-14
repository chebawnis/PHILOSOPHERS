# Makefile pour le projet philosophers
NAME     := philo
CC       := gcc
CFLAGS   := -Wall -Wextra -Werror -pthread
INCLUDES := -I .

SRCDIR := src
OBJDIR := objs

SRCS := main.c utils1.c utils2.c utils3.c utils4.c utils5.c
SRCS := $(addprefix $(SRCDIR)/,$(SRCS))
OBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c philosophers.h | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
