#include "lexer.h"

t_token	*new_token(t_token_type token_type, t_lexer *l, size_t len, size_t len_start)
{
	t_token	 *token;

	token = x_malloc(sizeof(*token));
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
	bool			closed;

	closed = true;
	token = x_malloc(sizeof(*token));
	while (!ft_strchr(DELIMITER, l->ch))
	{
		if (l->ch == '\'' || l->ch == '\"')
		{
			quote_type = l->ch;
			read_char(l);
			while (l->ch != quote_type && l->ch != '\0')
				read_char(l);
			if (l->ch == '\0')
				closed = false;
		}
		read_char(l);
		if (l->is_subshell && l->ch == '\n')
			break ;
	}
	if (closed)
		token->type = STRING;
	else
		token->type = ILLEGAL;
	token->literal.len = l->position - len_start;
	token->literal.start = &(l->input[len_start]);
	token->next = NULL;
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
	while (l->ch == '\n')
	{
		read_char(l);
		newline_num++;
	}
	if (l->is_subshell)
		newline_type = SUBSHELL_NEWLINE_MS;
	else if (l->is_redirect)
		newline_type = NEWLINE_MS;
	token = new_token(newline_type, l, newline_num, len_start);
	return (token);
}
