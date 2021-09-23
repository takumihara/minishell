#include "parser.h"

t_node *parse(t_lexer *lexer)
{
	t_parser		*p;
	t_token_type	type;
	t_node			*head;
	t_node			*itr;

	head = NULL;
	p = new_parser(lexer);
	if (!p)
		return (NULL);
	while (1)
	{
		type = p->token->type;
		if (type == STRING)
		{
			itr = parse_string(p);
			if (!itr)
				return (NULL);
		}
		else if (type == EOL)
		{
			itr = NULL;
			break ;
		}
		if (!head)
			head = itr;
		itr = itr->next;
	}
	free(p);
	return (head);
}

t_node *parse_string(t_parser *p)
{
	t_node *node;

	if (is_builtin(&p->token->literal))
		node = parse_builtin(p);
	else if (p->token->)

	return (node);
}

t_node *parse_builtin(t_parser *p)
{
	t_node *node;
	size_t len;

	// todo: free required
	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = BUILTIN;
	node->tokens->head = p->token;
	len = 0;
	while (!is_delim_token(p->token->type))
	{
		next_token(p->l);
		len++;
	}
	node->tokens->len = len;
	return (node);
}
