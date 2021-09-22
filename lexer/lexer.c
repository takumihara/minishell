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
	lexer->read_position = 0;
	return lexer;
}


t_token *next_token(t_lexer *lexer)
{
	t_token	*token;

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
		token = new_token(EOL, 4);
	else
	{
    // TODO: rm is_letter
		if (is_letter(lexer->ch))
		{
			token = (t_token *)malloc(sizeof(t_token));
			token->literal = read_identifier(lexer);
			token->type = lookup_ident(token->literal);
			return (token);
			// printf("%s\n", token.literal);
		}
    // 	else
// 		token = new_token_string(lexer);
	}
	read_char(lexer);
	return (token);
}