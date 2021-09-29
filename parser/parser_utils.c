#include "parser.h"

t_parser	*new_parser(t_token *token)
{
	t_parser	*parser;

	parser = malloc(sizeof(t_parser));
	if (!parser)
		return (NULL);
	parser->token = token;
	return (parser);
}

// move to next token if current token was expected token type
bool	consume_token(t_parser *p, t_token_type expected_type, t_ast_node *node)
{
	if (!p->token || p->token->type != expected_type)
		return (false);
	if (node)
		node->data = &p->token->literal;
	p->token = p->token->next;
	return (true);
}

t_ast_node	*route_expressions(t_parser *p, t_ast_node *(*f[])(t_parser *), int element_num)
{
	const t_token	*tmp = p->token;
	t_ast_node		*node;
	int 			i;

	i = -1;
	while (++i < element_num)
	{
		if (assign_ast_node(&node, f[i](p)))
			return (node);
		// todo: need to create test if p->token should be changed when returning NULL
		p->token = (t_token *)tmp;
	}
	return (NULL);
}

