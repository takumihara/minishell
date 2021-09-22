#include <printf.h>
#include "lexer.h"
#include "../libft/libft.h"

t_token new_token_string(t_lexer *lexer)
{
	size_t start;
	t_token token;

	start = lexer->position;
	while (!ft_strchr(DELIMITER, lexer->input[lexer->read_position]))
		read_char(lexer);

	token.type = STRING;
	ft_memmove(token.literal, &lexer->input[start], (lexer->read_position - start)*sizeof(char));
	return (token);
}

void	read_char(t_lexer *lexer)
{
	if (lexer->read_position >= ft_strlen(lexer->input)) {
		lexer->ch = 0;
	} else {
		lexer->ch = lexer->input[lexer->read_position];
	}
	lexer->ch = lexer->input[lexer->read_position];
	lexer->position = lexer->read_position;
	lexer->read_position++;
}

void	skip_space(t_lexer *lexer)
{
	while (ft_isspace(lexer->ch))
		read_char(lexer);
}

t_token	new_token(t_token_type token_type, char ch)
{
	t_token	 token;

	token.literal[0] = ch;
	token.literal[1] = '\0';
	token.type = token_type;
	return (token);
}