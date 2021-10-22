#include "lexer.h"
#include "internal/lexer_internal.h"

void	token_lstclear(t_token *lst)
{
	t_token	*tmp;

	if (!lst)
		return ;
	while (lst)
	{
		tmp = lst->next;
		free(lst);
		lst = tmp;
	}
}
