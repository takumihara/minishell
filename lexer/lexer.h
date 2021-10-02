#ifndef LEXER_H
# define LEXER_H

# define RESET   "\033[0m"
# define RED     "\033[31m"      /* Red */

# include "../token/token.h"
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
// opendir(), readdir() -> wildcard
# include <sys/types.h>
# include <dirent.h>
# include <stdbool.h>
// todo: remove this
# include "printf.h"

# define DELIMITER "|&<>() "

// todo: need to discuss if read_position is actually needed?
// todo: type of positions are different from the book
// position: current character
// read_point: next character
// ch: currently investigated character
typedef struct s_lexer
{
	char	*input;
	size_t	position;
	size_t	read_position;
	char	ch;
	bool	is_subshell;
	bool	is_redirect;
}	t_lexer;

typedef int	t_bool;
# define FALSE 0
# define TRUE 1

// lexer.c
t_lexer	*new_lexer(char *input);
t_token	*next_token(t_lexer *lexer);
t_token	*lex(char *input);

// lexer_utils.c
void	read_char(t_lexer *lexer);
t_token	*skip_space(t_lexer *lexer);
int		is_digit(char c);

// lexer_new_token.c
t_token	*new_token(t_token_type token_type, t_lexer *l, size_t len, size_t start);
t_token	*new_token_string(t_lexer *lexer);
t_token	*new_token_redirect_or_string(t_lexer *lexer);
t_token	*new_token_newline(t_lexer *lexer);

// lexer_list.c
void	token_lstclear(t_token *lst);

#endif //LEXER_H
