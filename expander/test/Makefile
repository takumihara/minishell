NAME = a.out

CURRENT_PATH		= .
EXPANDER_PATH		= ..
EXPANDER_INT_PATH	= ../internal
WRAPPER_PATH		= ../../wrapper
PARSER_PATH			= ../../parser
PARSER_INT_PATH		= ../../parser/internal
LEXER_PATH			= ../../lexer
LEXER_INT_PATH		= ../../lexer/internal
AST_PATH			= ../../ast
UTILS_PATH			= ../../utils
ENV_PATH			= ../../env

SRC_PATHS		= $(CURRENT_PATH) $(EXPANDER_PATH) $(EXPANDER_INT_PATH) $(WRAPPER_PATH) $(PARSER_PATH) $(PARSER_INT_PATH) $(LEXER_PATH) $(LEXER_INT_PATH) $(AST_PATH) $(UTILS_PATH) $(ENV_PATH)
SRCS			= $(foreach path, $(SRC_PATHS), $(wildcard $(path)/*.c))

VPATH		= $(CURRENT_PATH):$(EXPANDER_PATH):$(EXPANDER_INT_PATH):$(WRAPPER_PATH):$(PARSER_PATH):$(PARSER_INT_PATH):$(LEXER_PATH):$(LEXER_INT_PATH):$(AST_PATH):$(UTILS_PATH):$(ENV_PATH)

OBJDIR  = ./obj
OBJS    = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:%.c=%.o)))
LIBFT_PATH = ../../libft

CC		= gcc
# CFLAG	= -Wall -Wextra -Werror -fsanitize=address
CFLAG	= -Wall -Wextra -Werror

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

norm:
	norminette $(EXPANDER_PATH)/*.c $(EXPANDER_PATH)/*.h $(EXPANDER_INT_PATH)/*.c $(EXPANDER_INT_PATH)/*.h | grep Error

.PHONY: all re clean fclean run
