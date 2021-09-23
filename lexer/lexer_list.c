#include <printf.h>
#include "lexer.h"
#include "../libft/libft.h"

int	token_lstadd_back(t_token **lst, t_token *new)
{
	t_token	*last;

	if (!lst || !new)
		return (0);
	if (!*lst)
	{
		*lst = new;
		return (1);
	}
	last = token_lstlast(*lst);
	last->next = new;
	new->prev = last;
	return (1);
}

t_token	*token_lstlast(t_token *lst)
{
	t_token	*tmp;

	if (!lst)
		return (NULL);
	tmp = lst;
	if (lst)
		while (tmp->next != NULL)
			tmp = tmp->next;
	return (tmp);
}
