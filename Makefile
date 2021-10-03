NAME    	= minishell

EXECUTE_PATH	= execute
PARSER_PATH		= parser
LEXER_PATH		= lexer
AST_PATH		= ast
UTILS_PATH		= utils
SRC_PATHS		= . $(EXECUTE_PATH) $(PARSER_PATH) $(LEXER_PATH) $(AST_PATH) $(UTILS_PATH)
SRCS			= $(foreach path, $(SRC_PATHS), $(wildcard $(path)/*.c))

OBJDIR  	= ./obj
OBJS    	= $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.c=.o)))
LIBFT_PATH	= libft

CC      	= gcc
CFLAG   	= -Wall -Wextra -Werror -fsanitize=address

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
	$(CC) $(CFLAG) $^ -o $@ $(INCLUDE) $(LIB)

bonus: $(NAME)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAG) -o $@ -c $<

$(OBJDIR)/%.o: $(EXECUTE_PATH)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAG) -o $@ -c $<

$(OBJDIR)/%.o: $(PARSER_PATH)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAG) -o $@ -c $<

$(OBJDIR)/%.o: $(LEXER_PATH)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAG) -o $@ -c $<

$(OBJDIR)/%.o: $(AST_PATH)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAG) -o $@ -c $<

$(OBJDIR)/%.o: $(UTILS_PATH)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAG) -o $@ -c $<

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