NAME    = a.out

LEXER_PATH	= ..

SRCS  		= $(wildcard *.c)
LEXER_SRCS	= $(wildcard $(LEXER_PATH)/*.c)

OBJDIR  		= ./obj
LEXER_OBJDIR	= $(LEXER_PATH)/obj

OBJS    		= $(addprefix $(OBJDIR)/, $(notdir $(SRCS:%.c=%.o)))
LEXER_OBJS		= $(addprefix $(LEXER_OBJDIR)/, $(notdir $(LEXER_SRCS:%.c=%.o)))

CC      = gcc
# CFLAG   = -Wall -Wextra -Werror -fsanitize=address
CFLAG   = -Wall -Wextra -Werror

LIBFT_PATH = ../../libft
INCLUDE		= -I../../lexer -I../../token

#all: $(NAME)
all: run

$(NAME): $(OBJS) $(LEXER_OBJS)
	@make -C $(LIBFT_PATH)
	$(CC) $(CFLAG) $(INCLUDE) $^ -o $@ -L$(LIBFT_PATH) -lft

$(OBJDIR)/%.o: %.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAG) -o $@ -c $<

$(LEXER_OBJDIR)/%.o: $(LEXER_PATH)/%.c
	@mkdir -p $(LEXER_OBJDIR)
	$(CC) $(CFLAG) -o $@ -c $<

clean:
	@make fclean -C $(LIBFT_PATH)
	$(RM) -r $(OBJDIR)
	$(RM) -r $(LEXER_OBJDIR)

fclean: clean
	@make fclean -C $(LIBFT_PATH)
	$(RM) $(NAME)

re: fclean all

run: $(NAME)
	./$(NAME)

.PHONY: all re clean fclean run
