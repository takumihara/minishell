#include <printf.h>
#include "lexer.h"
#include "../libft/libft.h"

t_token	*new_token(t_token_type token_type, t_lexer *l, size_t len)
{
	t_token	 *token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = token_type;
	token->literal.start = &l->input[l->position];
	token->literal.len = len;
	token->prev = NULL;
	token->next = NULL;
	return (token);
}

t_token	*new_token_string(t_lexer *l)
{
	t_token	*token;
	char	*str_start;
	size_t	len_start;
	char	quote_type;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	str_start = &(l->input[l->position]);
	len_start = l->position;
	while (!ft_strchr(DELIMITER, l->ch))
	{
		if (l->ch == '\'' || l->ch == '\"')
		{
			quote_type = l->ch;
			read_char(l);
			while (l->ch != quote_type && l->ch != '\0')
				read_char(l);
		}
		read_char(l);
		if (l->is_subshell && l->ch == '\n')
			break ;
	}
	token->type = STRING;
	token->literal.len = l->position - len_start;
	token->literal.start = str_start;
	token->prev = NULL;
	token->next = NULL;
	return (token);
}

t_token	*new_token_environment(t_lexer *l)
{
	t_token	*token;
	char	*str_start;
	size_t	len_start;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	str_start = &(l->input[l->position]);
	len_start = l->position;
	read_char(l);
	while (!ft_strchr(DELIMITER, l->ch))
	{
		read_char(l);
		if (l->is_subshell && l->ch == '\n')
			break ;
	}
	token->type = ENVIRONMENT;
	token->literal.len = l->position - len_start;
	token->literal.start = str_start;
	token->prev = NULL;
	token->next = NULL;
	return (token);
}

t_token	*new_token_redirect_or_string(t_lexer *l)
{
	t_token	*token;
	size_t	len_start;
	size_t	digits;

	digits = 0;
	len_start = l->position;
	while (is_digit(l->ch))
	{
		read_char(l);
		digits++;
	}
	l->position = len_start;
	l->read_position = len_start + 1;
	if (l->ch == '<' || l->ch == '>')
	{
		token = new_token(REDIRECT_MODIFIER, l, digits);
		while (digits-- > 0)
			read_char(l);
	}
	else
	{
		l->ch = l->input[l->position];
		token = new_token_string(l);
	}
	return (token);
}

t_token	*new_token_newline(t_lexer *l)
{
	t_token	*token;
	size_t	len_start;
	size_t	newline_num;

	newline_num = 0;
	len_start = l->position;
	while (l->ch != '\n')
	{
		read_char(l);
		newline_num++;
	}
	l->position = len_start;
	l->read_position = len_start + 1;
	token = new_token(SUBSHELL_NEWLINE, l, newline_num);
	while (newline_num-- > 0)
		read_char(l);
	return (token);
}
