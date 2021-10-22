#ifndef LEXER_H
# define LEXER_H

# include "../token/token.h"

// lex.c
t_token	*lex(char *input);

// lexer_list.c
void	token_lstclear(t_token *lst);

#endif
