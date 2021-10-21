NAME    = a.out

CURRENT_PATH	= .
LEXER_PATH		= ..
UTILS_PATH		= ../../utils
WRAPPER_PATH	= ../../wrapper

SRC_PATHS	= $(CURRENT_PATH) $(LEXER_PATH) $(UTILS_PATH) $(WRAPPER_PATH)
SRCS		= $(foreach path, $(SRC_PATHS), $(wildcard $(path)/*.c))

VPATH		= $(CURRENT_PATH):$(LEXER_PATH):$(UTILS_PATH):$(WRAPPER_PATH)

OBJDIR  = ./obj
OBJS    = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:%.c=%.o)))

CC      = gcc
# CFLAG   = -Wall -Wextra -Werror -fsanitize=address
CFLAGS  = -Wall -Wextra -Werror

LIBFT_PATH	= ../../libft
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
