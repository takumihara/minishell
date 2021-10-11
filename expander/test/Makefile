NAME = a.out

CURRENT_PATH	= .
EXPANDER_PATH	= ..
PARSER_PATH		= ../../parser
LEXER_PATH		= ../../lexer
AST_PATH		= ../../ast
UTILS_PATH		= ../../utils
ENV_PATH		= ../../env

SRC_PATHS		= $(ENV_PATH) $(CURRENT_PATH) $(EXPANDER_PATH) $(BUILTIN_PATH) $(EXECUTE_PATH) $(PARSER_PATH) $(LEXER_PATH) $(AST_PATH) $(UTILS_PATH) 
SRCS			= $(foreach path, $(SRC_PATHS), $(wildcard $(path)/*.c))

VPATH		= $(CURRENT_PATH):$(BUILTIN_PATH):$(EXECUTE_PATH):$(PARSER_PATH):$(LEXER_PATH):$(AST_PATH):$(UTILS_PATH):$(ENV_PATH):$(EXPANDER_PATH)

OBJDIR  = ./obj
OBJS    = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:%.c=%.o)))
LIBFT_PATH = ../../libft

CC		= gcc
CFLAG	= -Wall -Wextra -Werror -fsanitize=address

LIB			= -L$(LIBFT_PATH) -lft
INCLUDE		=


all: $(NAME) run

$(NAME): $(OBJS)
	@make -C $(LIBFT_PATH)
	$(CC) $(CFLAG) $^ -o $@ $(INCLUDE) $(LIB)

$(OBJDIR)/%.o: %.c
	@if [ ! -d $(dir $@) ];then mkdir $(dir $@); fi
	$(CC) $(CFLAG) -o $@ -c $<

clean:
	@make fclean -C $(LIBFT_PATH)
	$(RM) -r $(OBJDIR)

fclean: clean
	@make fclean -C $(LIBFT_PATH)
	$(RM) $(NAME)

re: fclean all

run:
	./$(NAME)

.PHONY: all re clean fclean run
