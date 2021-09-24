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

t_token	*new_token_single_quote(t_lexer *lexer)
{
	t_token	*token;
	char	*str_start;
	size_t	len_start;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	read_char(lexer);
	str_start = &(lexer->input[lexer->position]);
	len_start = lexer->position;
	while (lexer->ch != '\'' && lexer->ch != '\0')
		read_char(lexer);
	token->literal.start = str_start;
	token->literal.len = lexer->position - len_start;
	if (lexer->ch == '\'')
		token->type = STRING;
	else if (lexer->ch == '\0')
		token->type = NOT_CLOSED;
	token->prev = NULL;
	token->next = NULL;
	return (token);
}
