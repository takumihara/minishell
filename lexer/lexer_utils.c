#include <printf.h>
#include "lexer.h"
#include "../libft/libft.h"

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

t_token	*skip_space(t_lexer *lexer)
{
	t_token	*token;

	token = NULL;
	while (ft_isspace(lexer->ch))
	{
		if (lexer->is_subshell && lexer->ch == '\n')
			token = new_token_newline(lexer);
		read_char(lexer);
	}
	return (token);
}

int	is_digit(char c)
{
	return ('0' <= c && c <= '9');
}

