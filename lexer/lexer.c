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
	lexer->position = 0;
	lexer->read_position = 1;
	lexer->ch = lexer->input[lexer->position];
	return lexer;
}


t_token next_token(t_lexer *lexer)
{
	t_token	token;

	skip_space(lexer);
	if (lexer->ch == '|')
	{
		if (lexer->input[lexer->read_position] == '|')
		{
			read_char(lexer);
			token.type = OR_IF;
			ft_memmove(token.literal, "||", 3);
		}
		else
			token = new_token(PIPE, lexer->ch);
	}
	else if (lexer->ch == '>')
		token = new_token(REDIRECT, lexer->ch);
	else if (lexer->ch == '&')
	{
		if (lexer->input[lexer->read_position] == '&')
		{
			read_char(lexer);
			token.type = AND_IF;
			ft_memmove(token.literal, "&&", 3);
		}
		else
			token = new_token(ILLEGAL, lexer->ch);
	}
	else if (lexer->ch == '\0')
	{
		token.type = EOL;
		// todo: different from the book
		token.literal[0] = 0;
	}
	else
		token = new_token_string(lexer);
//		token = new_token(ILLEGAL, lexer->ch);
// make sure lexer->position now indicates the last char of this tern
	read_char(lexer);
	return (token);
}