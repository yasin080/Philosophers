NAME		= philo
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread
HEADER		= philo.h

SRCS		= main.c \
			  parse.c \
			  init.c \
			  routine.c \
			  monitor.c \
			  print.c \
			  time.c

OBJDIR		= obj
OBJS		= $(SRCS:%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJDIR)/%.o: %.c $(HEADER) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
