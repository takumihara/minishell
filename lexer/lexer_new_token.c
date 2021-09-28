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
		if (lexer->is_subshell && lexer->ch == '\n')
			break ;
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
	{
		read_char(lexer);
		if (lexer->is_subshell && lexer->ch == '\n')
			break ;
	}
	token->type = ENVIRONMENT;
	token->literal.len = lexer->position - len_start;
	token->literal.start = str_start;
	token->prev = NULL;
	token->next = NULL;
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
		token = new_token(REDIRECT_MODIFIER, lexer, digits);
		while (digits-- > 0)
			read_char(lexer);
	}
	else
	{
		lexer->ch = lexer->input[lexer->position];
		token = new_token_string(lexer);
	}
	return (token);
}

t_token	*new_token_newline(t_lexer *lexer)
{
	t_token	*token;
	size_t	len_start;
	size_t	newline_num;

	newline_num = 0;
	len_start = lexer->position;
	while (lexer->ch != '\n')
	{
		read_char(lexer);
		newline_num++;
	}
	lexer->position = len_start;
	lexer->read_position = len_start + 1;
	token = new_token(SUBSHELL_NEWLINE, lexer, newline_num);
	while (newline_num-- > 0)
		read_char(lexer);
	return (token);
}
