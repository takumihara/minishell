#include <printf.h>
#include "lexer.h"
#include "../libft/libft.h"

t_token	*new_token(t_token_type token_type, t_lexer *l, size_t len, size_t len_start)
{
	t_token	 *token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = token_type;
	token->literal.start = &(l->input[len_start]);
	token->literal.len = len;
	token->next = NULL;
	if (token_type == REDIRECT_IN || token_type == REDIRECT_OUT
		|| token_type == HEREDOC || token_type == REDIRECT_APPEND)
		l->is_redirect = true;
	return (token);
}

t_token	*new_token_string(t_lexer *l)
{
	t_token			*token;
	const size_t	len_start = l->position;
	char			quote_type;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
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
	token->literal.start = &(l->input[len_start]);
	token->next = NULL;
	return (token);
}

t_token	*new_token_environment(t_lexer *l)
{
	t_token			*token;
	const size_t	len_start = l->position;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	read_char(l);
	while (!ft_strchr(DELIMITER, l->ch))
	{
		read_char(l);
		if (l->is_subshell && l->ch == '\n')
			break ;
	}
	token->type = ENVIRONMENT;
	token->literal.len = l->position - len_start;
	token->literal.start = &(l->input[len_start]);
	token->next = NULL;
	return (token);
}

t_token	*new_token_redirect_or_string(t_lexer *l)
{
	t_token			*token;
	const size_t	len_start = l->position;
	size_t			digits;

	digits = 0;
	while (is_digit(l->ch))
	{
		read_char(l);
		digits++;
	}
	if (l->ch == '<' || l->ch == '>')
		token = new_token(REDIRECT_MODIFIER, l, digits, len_start);
	else
	{
		l->position = len_start;
		l->read_position = len_start + 1;
		l->ch = l->input[len_start];
		token = new_token_string(l);
	}
	return (token);
}

t_token	*new_token_newline(t_lexer *l)
{
	t_token			*token;
	const size_t	len_start = l->position;
	size_t			newline_num;
	t_token_type	newline_type;

	newline_num = 0;
	newline_type = ILLEGAL;
	while (l->ch != '\n')
	{
		read_char(l);
		newline_num++;
	}
	if (l->is_subshell)
		newline_type = SUBSHELL_NEWLINE;
	else if (l->is_redirect)
		newline_type = NEWLINE;
	token = new_token(newline_type, l, newline_num, len_start);
	return (token);
}
