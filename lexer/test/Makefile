NAME    = a.out
SRCS    = $(wildcard *.c)
SRCS    = $(wildcard ../*.c)
SRCS	+= $(wildcard ../../token/*.c)
OBJDIR  = ./obj
OBJS    = $(addprefix $(OBJDIR)/, $(SRCS:%.c=%.o))
CC      = gcc
CFLAG   = -Wall -Wextra -Werror -fsanitize=address

LIBFT_PATH = ../../libft
INCLUDE		= -I../../lexer -I../../token

#all: $(NAME)
all: run

$(NAME): $(OBJS)
	@make -C $(LIBFT_PATH)
	$(CC) $(CFLAG) $(INCLUDE) $^ -o $@ -L$(LIBFT_PATH) -lft

$(OBJDIR)/%.o: %.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAG) -o $@ -c $<

clean:
	@make fclean -C $(LIBFT_PATH)
	$(RM) -r $(OBJDIR)

fclean: clean
	@make fclean -C $(LIBFT_PATH)
	$(RM) $(NAME)

re: fclean all

run: $(NAME)
	./$(NAME)

.PHONY: all re clean fclean run
