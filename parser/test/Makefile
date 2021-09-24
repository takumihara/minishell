NAME = a.out

PARSER_PATH	= ..
LEXER_PATH	= ../../lexer
SRC_PATHS	= . $(PARSER_PATH) $(LEXER_PATH)
SRCS		= $(foreach path, $(SRC_PATHS), $(wildcard $(path)/*.c))

OBJDIR  = ./obj
OBJS    = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:%.c=%.o)))

CC		= gcc
CFLAG	= -Wall -Wextra -Werror -fsanitize=address

LIBFT_PATH = ../../libft
#INCLUDE		= -I../../lexer -I../../token -I../../ast -I../../parser

#all: $(NAME)
all: run

$(NAME): $(OBJS)
	@make -C $(LIBFT_PATH)
	$(CC) $(CFLAG) $(INCLUDE) $^ -o $@ -L$(LIBFT_PATH) -lft

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAG) -o $@ -c $<

$(OBJDIR)/%.o: $(PARSER_PATH)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAG) -o $@ -c $<

$(OBJDIR)/%.o: $(LEXER_PATH)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAG) -o $@ -c $<

clean:
	@make fclean -C $(LIBFT_PATH)
	$(RM) -r $(OBJDIR)
	$(RM) -r $(PARSER_OBJDIR)
	$(RM) -r $(LEXER_OBJDIR)

fclean: clean
	@make fclean -C $(LIBFT_PATH)
	$(RM) $(NAME)

re: fclean all

run: $(NAME)
	./$(NAME)

.PHONY: all re clean fclean run
