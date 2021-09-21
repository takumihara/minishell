#ifndef LEXER_H
# define LEXER_H

# include "../token/token.h"
# include <stdlib.h>
# include <stdio.h>

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

// lexer.c
t_lexer	*new_lexer(char *input);
t_token	*next_token(t_lexer *lexer);

// lexer_utils.c
void	read_char(t_lexer *lexer);
void	skip_space(t_lexer *lexer);
t_token	*new_token(t_token_type token_type, char ch);
int		is_letter(char ch);
char	*read_identifier(t_lexer *lexer);
enum e_token_type	lookup_ident(char *ident);

#endif //LEXER_H
