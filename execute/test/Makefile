NAME = a.out

CURRENT_PATH	= .
EXECUTE_PATH	= ..
EXECUTE_INT_PATH= ../internal
WRAPPER_PATH	= ../../wrapper
EXPANDER_PATH	= ../../expander
EXPANDER_INT_PATH= ../../expander/internal
PARSER_PATH		= ../../parser
PARSER_INT_PATH	= ../../parser/internal
LEXER_PATH		= ../../lexer
LEXER_INT_PATH	= ../../lexer/internal
AST_PATH		= ../../ast
UTILS_PATH		= ../../utils
BUILTIN_PATH	= ../../builtin
ENV_PATH		= ../../env

SRC_PATHS		= $(CURRENT_PATH) $(EXPANDER_PATH) $(EXPANDER_INT_PATH) $(WRAPPER_PATH) $(BUILTIN_PATH) $(EXECUTE_PATH) $(EXECUTE_INT_PATH) $(PARSER_PATH) $(PARSER_INT_PATH) $(LEXER_PATH) $(LEXER_INT_PATH) $(AST_PATH) $(UTILS_PATH) $(ENV_PATH)
SRCS			= $(foreach path, $(SRC_PATHS), $(wildcard $(path)/*.c))

VPATH		= $(CURRENT_PATH):$(BUILTIN_PATH):$(EXECUTE_PATH):$(EXECUTE_INT_PATH):$(WRAPPER_PATH):$(PARSER_PATH):$(PARSER_INT_PATH):$(LEXER_PATH):$(LEXER_INT_PATH):$(AST_PATH):$(UTILS_PATH):$(ENV_PATH):$(EXPANDER_PATH):$(EXPANDER_INT_PATH)

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
	norminette $(EXECUTE_PATH)/*.c $(EXECUTE_PATH)/*.h $(EXECUTE_INT_PATH)/*.c $(EXECUTE_INT_PATH)/*.h | grep Error

.PHONY: all re clean fclean run norm
