NAME = a.out

CURRENT_PATH	= .
EXECUTE_PATH	= ..
EXE_WRAP_PATH	= ../wrapper
EXPANDER_PATH	= ../../expander
PARSER_PATH		= ../../parser
LEXER_PATH		= ../../lexer
AST_PATH		= ../../ast
UTILS_PATH		= ../../utils
BUILTIN_PATH	= ../../builtin
ENV_PATH		= ../../env

SRC_PATHS		= $(CURRENT_PATH) $(EXPANDER_PATH) $(EXE_WRAP_PATH) $(BUILTIN_PATH) $(EXECUTE_PATH) $(PARSER_PATH) $(LEXER_PATH) $(AST_PATH) $(UTILS_PATH) $(ENV_PATH)
SRCS			= $(foreach path, $(SRC_PATHS), $(wildcard $(path)/*.c))

VPATH		= $(CURRENT_PATH):$(BUILTIN_PATH):$(EXECUTE_PATH):$(EXE_WRAP_PATH):$(PARSER_PATH):$(LEXER_PATH):$(AST_PATH):$(UTILS_PATH):$(ENV_PATH):$(EXPANDER_PATH)

OBJDIR  = ./obj
OBJS    = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:%.c=%.o)))

CC		= gcc
CFLAG	= -Wall -Wextra -Werror #-fsanitize=address

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

norm:
	norminette $

.PHONY: all re clean fclean run
