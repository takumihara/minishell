NAME    	= minishell

CURRENT_PATH	= .
EXECUTE_PATH	= execute
EXE_WRAP_PATH	= execute/wrapper
EXPANDER_PATH	= expander
PARSER_PATH		= parser
LEXER_PATH		= lexer
AST_PATH		= ast
UTILS_PATH		= utils
BUILTIN_PATH	= builtin
ENV_PATH		= env

SRC_PATHS		= $(CURRENT_PATH) $(EXPANDER_PATH) $(EXE_WRAP_PATH) $(BUILTIN_PATH) $(EXECUTE_PATH) $(PARSER_PATH) $(LEXER_PATH) $(AST_PATH) $(UTILS_PATH) $(ENV_PATH)
SRCS			= $(foreach path, $(SRC_PATHS), $(wildcard $(path)/*.c))

VPATH		= $(CURRENT_PATH):$(BUILTIN_PATH):$(EXECUTE_PATH):$(EXE_WRAP_PATH):$(PARSER_PATH):$(LEXER_PATH):$(AST_PATH):$(UTILS_PATH):$(ENV_PATH):$(EXPANDER_PATH)

OBJDIR  	= ./obj
OBJS    	= $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.c=.o)))
LIBFT_PATH	= libft

CC      	= gcc
CFLAGS   	= -Wall -Wextra -Werror -fsanitize=address

LIB			= -L$(LIBFT_PATH) -lft
INCLUDE		=

ifeq ($(shell uname), Darwin)
	INCLUDE += -I$(shell brew --prefix readline)/include
	LIB += -L$(shell brew --prefix readline)/lib -lreadline
else
	 LIB += -lreadline
endif

all: $(NAME) run

$(NAME): $(OBJS)
	@make -C $(LIBFT_PATH)
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDE) $(LIB)

bonus: $(NAME)

$(OBJDIR)/%.o: %.c
	@if [ ! -d $(dir $@) ];then mkdir $(dir $@); fi
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

clean:
	@make clean -C $(LIBFT_PATH)
	@rm -rf $(OBJDIR)

fclean: clean
	@make fclean -C $(LIBFT_PATH)
	@rm -f $(NAME) $(CHECKER)

re: fclean all

run:
	./$(NAME)

.PHONY: all bonus clean fclean re run