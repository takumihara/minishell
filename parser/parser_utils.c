#include "parser.h"

t_parser	*new_parser(t_token *token)
{
	t_parser	*parser;

	parser = malloc(sizeof(t_parser));
	if (!parser)
		return (NULL);
	parser->token = token;
	parser->err = 0;
	parser->is_subshell = false;
	return (parser);
}

// move to next token if current token was expected token type
bool	consume_token(t_parser *p, t_token_type expected_type, t_ast_node *node)
{
	if (p->token->type != expected_type)
		return (false);
	if (node)
	{
		node->data = ft_strndup(p->token->literal.start, p->token->literal.len);
		if (!node->data)
			perror_exit("malloc", EXIT_FAILURE);
	}
	p->token = p->token->next;
	return (true);
}

t_ast_node	*route_expressions(t_parser *p, t_ast_node *(*f[])(t_parser *), int element_num)
{
	const t_token	*tmp = p->token;
	t_ast_node		*node;
	int				i;

	i = -1;
	while (++i < element_num)
	{
		if (assign_ast_node(&node, f[i](p)))
			return (node);
		if (p->err)
			return (NULL);
		// todo: need to create test if p->token should be changed when returning NULL
		p->token = (t_token *)tmp;
	}
	return (NULL);
}

#ifdef TEST

char	*handle_err(t_parser *p)
{
	char	*rtn;

	rtn = NULL;
	if (p->err == ERR_UNEXPECTED_TOKEN)
	{
		rtn = ft_strdup("minishell: syntax error near unexpected token `");
		if (p->token->type == NEWLINE_MS || p->token->type == SUBSHELL_NEWLINE_MS || p->token->type == EOL)
			rtn = strappend(rtn, "newline", 7);
		else
			rtn = strappend(rtn, p->token->literal.start, p->token->literal.len);
		rtn = strappend(rtn, "'\n", 2);
	}
	else if (p->err == ERR_UNEXPECTED_EOF)
		rtn = ft_strdup("minishell: syntax error: unexpected end of file\n");
	return (rtn);
}

#else

bool	handle_err(t_parser *p)
{
	if (p->err == ERR_UNEXPECTED_TOKEN)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
		if (p->token->type == NEWLINE_MS || p->token->type == SUBSHELL_NEWLINE_MS || p->token->type == EOL)
			ft_putstr_fd("newline", STDERR_FILENO);
		else
			write(STDERR_FILENO, p->token->literal.start, p->token->literal.len);
		ft_putendl_fd("'", STDERR_FILENO);
	}
	else if (p->err == ERR_UNEXPECTED_EOF)
		ft_putendl_fd("minishell: syntax error: unexpected end of file", STDERR_FILENO);
	if (p->err)
		return (true);
	return (false);
}

#endif
