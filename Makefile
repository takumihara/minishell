NAME    	= minishell
SRCS		:= $(wildcard *.c)
OBJDIR  	= ./obj
OBJS    	= $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.c=.o)))
LIBFT_PATH	= libft

CC      	= gcc
CFLAG   	= -Wall -Wextra -Werror

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
	@-mkdir -p $(OBJDIR)
	$(CC) $(CFLAG) $(INCLUDE) -o $@ -c $<

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