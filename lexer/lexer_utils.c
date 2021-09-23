#include <printf.h>
#include "lexer.h"
#include "../libft/libft.h"

t_token	*new_token_string(t_lexer *lexer)
{
	t_token	*token;
	char	*str;
	size_t	start;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	str = &(lexer->input[lexer->position]);
	printf("str:%s\n", str);
	start = lexer->position;
	while (!ft_strchr(DELIMITER, lexer->ch))
		read_char(lexer);
	token->type = STRING;
	token->literal.len = lexer->position - start;
	token->literal.str = str;
	return (token);
}

void	read_char(t_lexer *lexer)
{
	if (lexer->read_position == ft_strlen(lexer->input))
		lexer->ch = 0;
	else if (lexer->read_position > ft_strlen(lexer->input))
		return ;
	else
		lexer->ch = lexer->input[lexer->read_position];
	lexer->ch = lexer->input[lexer->read_position];
	lexer->position = lexer->read_position;
	lexer->read_position++;
}

void	skip_space(t_lexer *lexer)
{
	while (ft_isspace(lexer->ch))
		read_char(lexer);
}

t_token	*new_token(t_token_type token_type, t_lexer *lexer, size_t len)
{
	t_token	 *token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = token_type;
	token->literal.str = &lexer->input[lexer->position];
	token->literal.len = len;
	return (token);
}
