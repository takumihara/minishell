#include "internal/internal.h"
#include <stdio.h>

#ifdef TEST

void	*parse(t_token *token)
{
	t_token		*head;
	t_parser	*p;
	t_ast_node	*node;
	char		*err_msg;

	head = token;
	p = new_parser(token);
	node = command_line(p);
	err_msg = handle_err(p);
	token_lstclear(head);
	free(p);
	if (err_msg)
	{
		delete_ast_nodes(node, NULL);
		return ((void *) err_msg);
	}
	return ((void *)node);
}

#else

t_ast_node	*parse(t_token *token)
{
	t_token			*head;
	t_parser		*p;
	t_ast_node		*root;

	head = token;
	p = new_parser(token);
	root = command_line(p);
	if (handle_err(p))
	{
		delete_ast_nodes(root, NULL);
		root = NULL;
	}
	token_lstclear(head);
	free(p);
	return (root);
}

#endif
