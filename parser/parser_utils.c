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

int	is_builtin(t_string *literal)
{
	if (literal->len == 4 && !ft_strncmp(literal->start, "echo", literal->len))
		return (1);
	if (literal->len == 2 && !ft_strncmp(literal->start, "cd", literal->len))
		return (1);
	if (literal->len == 3 && !ft_strncmp(literal->start, "pwd", literal->len))
		return (1);
	if (literal->len == 6 && !ft_strncmp(literal->start, "export", literal->len))
		return (1);
	if (literal->len == 5 && !ft_strncmp(literal->start, "unset", literal->len))
		return (1);
	if (literal->len == 3 && !ft_strncmp(literal->start, "env", literal->len))
		return (1);
	if (literal->len == 4 && !ft_strncmp(literal->start, "exit", literal->len))
		return (1);
	return (0);
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
