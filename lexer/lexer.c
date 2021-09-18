#include "lexer.h"
#include "../libft/libft.h"

t_lexer *new_lexer(char *input)
{
	t_lexer *lexer;

	// todo: FREE required!!
	lexer = malloc(sizeof(t_lexer));
	if (!lexer)
		return (NULL);
	lexer->input = input;
	return lexer;
}


t_token next_token(t_lexer *lexer)
{
	t_token	token;

	skip_space(lexer);
	if (lexer->ch == '|')
		token = new_token(PIPE, lexer->ch);
	else if (lexer->ch == '\0')
	{
		token.type = EOL;
		// todo: different from the book
		token.literal[0] = 4;
	}
	else
		token = new_token(ILLEGAL, lexer->ch);
//	read_char(lexer);
	return (token);
}