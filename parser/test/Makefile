NAME = a.out

CUR_PATH	= .
PARSER_PATH	= ..
PARSER_INT_PATH	= ../internal
LEXER_PATH	= ../../lexer
LEXER_INT_PATH= ../../lexer/internal
AST_PATH	= ../../ast
UTILS_PATH	= ../../utils
WRAPPER_PATH= ../../wrapper

SRC_PATHS	= $(CUR_PATH) $(PARSER_PATH) $(PARSER_INT_PATH) $(LEXER_PATH) $(LEXER_INT_PATH) $(AST_PATH) $(UTILS_PATH) $(WRAPPER_PATH)
SRCS		= $(foreach path, $(SRC_PATHS), $(wildcard $(path)/*.c))

VPATH		= $(CUR_PATH):$(PARSER_PATH):$(PARSER_INT_PATH):$(LEXER_PATH):$(LEXER_INT_PATH):$(AST_PATH):$(UTILS_PATH):$(WRAPPER_PATH)

OBJDIR  = ./obj
OBJS    = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:%.c=%.o)))

CC		= gcc
CFLAG	= -Wall -Wextra -Werror -DTEST #-fsanitize=address

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

norm:
	norminette $(PARSER_PATH)/*.c $(PARSER_PATH)/*.h $(PARSER_INT_PATH)/*.c $(PARSER_INT_PATH)/*.h| grep Error

.PHONY: all re clean fclean run norm
