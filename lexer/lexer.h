#ifndef LEXER_H
# define LEXER_H

# include "../token/token.h"
# include <stdlib.h>
# include <stdio.h>
// todo: remove this
# include "printf.h"

# define DELIMITER "|&<>=$() "

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
}	t_lexer;

typedef int	t_bool;
# define FALSE 0
# define TRUE 1

// lexer.c
t_lexer	*new_lexer(char *input);
t_token	*next_token(t_lexer *lexer);
t_token	*lexer_main(t_lexer *lexer);

// lexer_utils.c
void	read_char(t_lexer *lexer);
void	skip_space(t_lexer *lexer);

// lexer_new_token.c
t_token	*new_token(t_token_type token_type, t_lexer *lexer, size_t len);
t_token	*new_token_string(t_lexer *lexer);
t_token	*new_token_environment(t_lexer *lexer);
t_token	*new_token_single_quote(t_lexer *lexer);
t_token	*new_token_double_quote(t_lexer *lexer);

// lexer_list.c
int		token_lstadd_back(t_token **lst, t_token *new);
t_token	*token_lstlast(t_token *lst);
void	token_lstclear(t_token **lst);

#endif //LEXER_H
