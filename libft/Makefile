NAME    = libft.a
SRCS    = $(wildcard *.c)
OBJDIR  = ./obj
OBJS    = $(addprefix $(OBJDIR)/, $(SRCS:%.c=%.o))
CC      = gcc
CFLAG   = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS)
	ar rc $(NAME) $(OBJS)
	ranlib $(NAME)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAG) -o $@ -c $<

clean:
	rm -f $(OBJS) $(B_OBJS)
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all re clean fclean