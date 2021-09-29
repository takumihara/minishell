#include <printf.h>
#include "lexer.h"
#include "../libft/libft.h"

void	read_char(t_lexer *l)
{
	if (l->read_position == ft_strlen(l->input))
		l->ch = 0;
	else if (l->read_position > ft_strlen(l->input))
		return ;
	else
		l->ch = l->input[l->read_position];
	l->ch = l->input[l->read_position];
	l->position = l->read_position;
	l->read_position++;
}

t_token	*skip_space(t_lexer *l)
{
	t_token	*token;

	token = NULL;
	while (ft_isspace(l->ch))
	{
		if ((l->is_subshell || l->is_redirect) && l->ch == '\n')
			token = new_token_newline(l);
		read_char(l);
	}
	l->is_redirect = false;
	if (l->ch == ')')
	{
		free(token);
		token = NULL;
	}
	return (token);
}

int	is_digit(char c)
{
	return ('0' <= c && c <= '9');
}
