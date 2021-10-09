NAME = a.out

CUR_PATH	= .
PARSER_PATH	= ..
LEXER_PATH	= ../../lexer
AST_PATH	= ../../ast
UTILS_PATH	= ../../utils
SRC_PATHS	= $(CUR_PATH) $(PARSER_PATH) $(LEXER_PATH) $(AST_PATH) $(UTILS_PATH)
SRCS		= $(foreach path, $(SRC_PATHS), $(wildcard $(path)/*.c))

VPATH		= $(CUR_PATH):$(PARSER_PATH):$(LEXER_PATH):$(AST_PATH):$(UTILS_PATH)

OBJDIR  = ./obj
OBJS    = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:%.c=%.o)))

CC		= gcc
CFLAG	= -Wall -Wextra -Werror -fsanitize=address -DTEST

LIBFT_PATH = ../../libft
#INCLUDE		= -I../../lexer -I../../token -I../../ast -I../../parser

#all: $(NAME)
all: run

$(NAME): $(OBJS)
	@make -C $(LIBFT_PATH)
	$(CC) $(CFLAG) $(INCLUDE) $^ -o $@ -L$(LIBFT_PATH) -lft

$(OBJDIR)/%.o: %.c
	@if [ ! -d $(dir $@) ];then mkdir $(dir $@); fi
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
