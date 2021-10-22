#include "lexer.h"

t_token	*lex(char *input)
{
	t_token	token;
	t_token	*tmp;
	t_lexer	*l;

	new_lexer(&l, input);
	token.next = NULL;
	tmp = &token;
	read_char(l);
	while (1)
	{
		tmp->next = next_token(l);
		if (tmp->next->type == EOL)
			break ;
		tmp = tmp->next;
	}
	free(l);
	return (token.next);
}
