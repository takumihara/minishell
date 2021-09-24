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

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	str_start = &(lexer->input[lexer->position]);
	len_start = lexer->position;
	while (!ft_strchr(DELIMITER, lexer->ch))
		read_char(lexer);
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