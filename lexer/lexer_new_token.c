#include <printf.h>
#include "lexer.h"
#include "../libft/libft.h"

t_token	*new_token(t_token_type token_type, t_lexer *lexer, size_t len)
{
	t_token	 *token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = token_type;
	token->literal.start = &lexer->input[lexer->position];
	token->literal.len = len;
	token->prev = NULL;
	token->next = NULL;
	return (token);
}

t_token	*new_token_string(t_lexer *lexer)
{
	t_token	*token;
	char	*str_start;
	size_t	len_start;
	char	quote_type;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	str_start = &(lexer->input[lexer->position]);
	len_start = lexer->position;
	while (!ft_strchr(DELIMITER, lexer->ch))
	{
		if (lexer->ch == '\'' || lexer->ch == '\"')
		{
			quote_type = lexer->ch;
			read_char(lexer);
			while (lexer->ch != quote_type && lexer->ch != '\0')
				read_char(lexer);
		}
		read_char(lexer);
	}
	token->type = STRING;
	token->literal.len = lexer->position - len_start;
	token->literal.start = str_start;
	token->prev = NULL;
	token->next = NULL;
	return (token);
}

t_token	*new_token_environment(t_lexer *lexer)
{
	t_token	*token;
	char	*str_start;
	size_t	len_start;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	str_start = &(lexer->input[lexer->position]);
	len_start = lexer->position;
	read_char(lexer);
	while (!ft_strchr(DELIMITER, lexer->ch))
		read_char(lexer);
	token->type = ENVIRONMENT;
	token->literal.len = lexer->position - len_start;
	token->literal.start = str_start;
	token->prev = NULL;
	token->next = NULL;
	return (token);
}

t_token	*new_token_redirect(t_lexer *lexer, size_t digits)
{
	t_token	*token;

	token = NULL;
	if (lexer->input[lexer->position + digits] == '>')
	{
		if (lexer->input[lexer->read_position + digits] == '>')
			token = new_token(REDIRECT_APPEND, lexer, digits + 2);
		else
			token = new_token(REDIRECT_OUT, lexer, digits + 1);
	}
	if (lexer->input[lexer->position + digits] == '<')
	{
		if (lexer->input[lexer->read_position + digits] == '<')
			token = new_token(HEREDOC, lexer, digits + 2);
		else
			token = new_token(REDIRECT_IN, lexer, digits + 1);
	}
	lexer->position += digits;
	lexer->read_position += digits + 1;
	if (token->type == REDIRECT_APPEND || token->type == HEREDOC)
		read_char(lexer);
	return (token);
}

t_token	*new_token_redirect_or_string(t_lexer *lexer)
{
	t_token	*token;
	size_t	len_start;
	size_t	digits;

	digits = 0;
	len_start = lexer->position;
	while (is_digit(lexer->ch))
	{
		read_char(lexer);
		digits++;
	}
	lexer->position = len_start;
	lexer->read_position = len_start + 1;
	if (lexer->ch == '<' || lexer->ch == '>')
	{
		token = new_token_redirect(lexer, digits);
		read_char(lexer);
	}
	else
	{
		lexer->ch = lexer->input[lexer->position];
		token = new_token_string(lexer);
	}
	return (token);
}
