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

SRCS = \
    wrapper/x_malloc.c \
    wrapper/x_dup.c \
    wrapper/x_readdir.c \
    wrapper/x_dup2.c \
    wrapper/x_strdup.c \
    wrapper/x_stat.c \
    wrapper/x_get_next_line.c \
    wrapper/x_substr.c \
    wrapper/x_signal.c \
    wrapper/x_split.c \
    wrapper/x_itoa.c \
    wrapper/x_fork.c \
    wrapper/x_pipe.c \
    wrapper/x_closedir.c \
    wrapper/x_opendir.c \
    wrapper/x_strndup.c \
    lexer/lexer_list.c \
    lexer/lexer.c \
    lexer/internal/lexer_new_token.c \
    lexer/internal/lexer_utils.c \
    lexer/internal/next_token.c \
    utils/strappend.c \
    utils/split_by_delims.c \
    utils/assign_mem.c \
    utils/read_all.c \
    utils/ft_strstr.c \
    utils/quick_sort_str.c \
    utils/get_next_line.c \
    utils/atoi_strict.c \
    utils/strjoin_three.c \
    utils/perror_exit.c \
    utils/ft_strndup.c \
    utils/free_2d_array.c \
    parser/internal/parser_utils.c \
    parser/internal/command_line.c \
    parser/internal/simple_command.c \
    parser/parser.c \
    env/register_env_var_from_literal.c \
    env/env_list.c \
    env/env_utils.c \
    expander/expander.c \
    expander/internal/expander_remove_quotes.c \
    expander/internal/expander_quote_utils.c \
    expander/internal/expander_wildcard.c \
    expander/internal/expander_wildcard_star_utils.c \
    expander/internal/expander_wildcard_utils.c \
    expander/internal/expander_env.c \
    expander/internal/expander_env_utils.c \
    expander/internal/expander_word_splitting_utils.c \
    expander/internal/expander_error.c \
    expander/internal/expander_utils.c \
    expander/internal/expander_word_splitting.c \
    ast/ast.c \
    execute/execute.c \
    execute/internal/execute_utils.c \
    execute/internal/eval.c \
    execute/internal/create_envp.c \
    execute/internal/execute_simple_command.c \
    execute/internal/eval_utils.c \
    execute/internal/execute_pipeline.c \
    execute/internal/execute_command_line.c \
    execute/internal/split_path_from_env.c \
    execute/internal/eval_utils_redirect.c \
    execute/internal/get_cmd_path.c \
    execute/internal/execute_command.c \
    execute/builtin/builtin_echo.c \
    execute/builtin/builtin_export.c \
    execute/builtin/builtin_unset.c \
    execute/builtin/builtin_pwd.c \
    execute/builtin/builtin_env.c \
    execute/builtin/builtin_exit.c \
    execute/builtin/builtin_cd.c \
    execute/builtin/is_valid_argument.c \
    signal/signal.c \
    minishell.c


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

all: $(NAME)

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

test: $(NAME)
	./$(NAME)

norm:
	norminette $(SRCS) $(HEADERS) | grep -E '^(Error|Warning)'

leaks:
	$(MAKE) CFLAGS="$(CFLAGS) -D LEAKS=1" LEAKS=TRUE

.PHONY: all bonus clean fclean re