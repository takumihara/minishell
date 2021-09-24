#include "parser.h"

static t_node *parse_command_and_args(t_parser *p);
static t_node *parse_string(t_parser *p);

t_node *parse(t_token *token)
{
	t_parser		*p;
	t_token_type	type;
	t_node			*head;
	t_node			*itr;

	head = NULL;
	p = new_parser(token);
	if (!p)
		return (NULL);
	// todo: after EOL token was implemented, remove (p->token)
	while (p->token)
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

	node = NULL;
	if (is_builtin(&p->token->literal))
	{
		node = parse_command_and_args(p);
		node->type = BUILTIN;
	}
	// todo: ig creation of environment variable
	else if (p->token->next->type == ASSIGN)
		node = NULL;
	else
	{
		node = parse_command_and_args(p);
		node->type = PROGRAM;
	}
	return (node);
}

t_node *parse_command_and_args(t_parser *p)
{
	t_node *node;
	size_t len;

	// todo: free required
	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	//todo: free required!
	node->tokens = malloc(sizeof(t_tokens));
	if (!node->tokens)
		return (NULL);
	node->tokens->head = p->token;
	len = 0;
	// todo: remove (p->token) after EOL token inplemented
	while (p->token && !is_delim_token(p->token->type))
	{
		p->token = p->token->next;
		len++;
	}
	node->tokens->len = len;
	return (node);
}
