NAME    	= minishell
NAME_LEAKS	= minishell_leaks
CURRENT_PATH		= .
EXECUTE_PATH		= execute
BUILTIN_PATH		= execute/builtin
EXECUTE_INT_PATH	= execute/internal
WRAPPER_PATH		= wrapper
EXPANDER_PATH		= expander
EXPANDER_INT_PATH	= expander/internal
PARSER_PATH			= parser
PARSER_INT_PATH		= parser/internal
LEXER_PATH			= lexer
LEXER_INT_PATH		= lexer/internal
AST_PATH			= ast
UTILS_PATH			= utils
ENV_PATH			= env
SIGNAL_PATH			= signal
TESTER_PATH			= test

SRC_PATHS		= $(CURRENT_PATH) $(EXPANDER_PATH) $(EXPANDER_INT_PATH) $(WRAPPER_PATH) $(BUILTIN_PATH) $(EXECUTE_PATH) $(EXECUTE_INT_PATH) $(PARSER_PATH) $(PARSER_INT_PATH) $(LEXER_PATH) $(LEXER_INT_PATH) $(AST_PATH) $(UTILS_PATH) $(ENV_PATH) $(SIGNAL_PATH)
SRCS			= $(foreach path, $(SRC_PATHS), $(wildcard $(path)/*.c))

HEADERS	= \
	minishell.h \
	ast/ast.h \
	env/env.h \
	execute/builtin/builtin.h \
	execute/internal/eval.h \
	execute/internal/eval_utils.h \
	execute/internal/execute_internal.h \
	execute/internal/split_path_from_env.h \
	execute/execute.h \
	execute/exit_status.h \
	expander/internal/expander_internal.h \
	expander/expander.h \
	lexer/internal/lexer_internal.h \
	lexer/lexer.h \
	libft/libft.h \
	parser/internal/parser_internal.h \
	parser/parser.h \
	token/token.h \
	utils/get_next_line.h \
	utils/utils.h \
	wrapper/x.h \
	signal/signal.h

VPATH		= $(CURRENT_PATH):$(BUILTIN_PATH):$(EXECUTE_PATH):$(EXECUTE_INT_PATH):$(WRAPPER_PATH):$(PARSER_PATH):$(PARSER_INT_PATH):$(LEXER_PATH):$(LEXER_INT_PATH):$(AST_PATH):$(UTILS_PATH):$(ENV_PATH):$(EXPANDER_PATH):$(EXPANDER_INT_PATH):$(TESTER_PATH):$(SIGNAL_PATH)

OBJDIR  	= ./obj
OBJS    	= $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.c=.o)))
LIBFT_PATH	= libft

CC      	= gcc
CFLAGS   	= -Wall -Wextra -Werror #-fsanitize=address

LIB			= -L$(LIBFT_PATH) -lft
INCLUDE		=

LEAKS_SRC	:= test/leaks.c

ifdef LEAKS
SRCS		+= test/leaks.c
NAME		:= $(NAME_LEAKS)
endif

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

norm:
	norminette $(SRCS) $(HEADERS) | grep Error

leaks:
	$(MAKE) CFLAGS="$(CFLAGS) -D LEAKS=1" LEAKS=TRUE

.PHONY: all bonus clean fclean re run