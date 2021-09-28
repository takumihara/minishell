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

void	token_lstclear(t_token **lst)
{
	t_token	*tmp;

	if (!lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		free(*lst);
		*lst = NULL;
		*lst = tmp;
	}
	lst = NULL;
}

// t_token    *token_lstmap(t_token *token, t_token *(*f)(t_lexer *))
// {
//     t_token	rtn;
//     t_token	*tmp;

//     rtn.next = NULL;
//     tmp = &rtn;
//     if (!lst || !f)
//         return (NULL);
//     while (lst)
//     {
//         tmp->next = ft_lstnew(f(lst->content));
//         if (!tmp->next)
//         {
//             ft_lstclear(&rtn.next, del);
//             return (NULL);
//         }
//         tmp = tmp->next;
//         lst = lst->next;
//     }
//     return (rtn.next);
// }
