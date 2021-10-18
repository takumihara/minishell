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
		{
			p->err = ERR_MALLOC;
			return (false);
		}
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

char	*handle_err(t_parser *p, t_ast_node *root)
{
	char	*rtn;

	if (p->err == ERR_UNEXPECTED_TOKEN)
	{
		rtn = ft_strdup("minishell: syntax error near unexpected token `");
		if (p->token->type == NEWLINE_MS || p->token->type == SUBSHELL_NEWLINE_MS)
			rtn = strappend(rtn, "newline", 7);
		else
			rtn = strappend(rtn, p->token->literal.start, p->token->literal.len);
		rtn = strappend(rtn, "'\n", 2);
	}
	else if (p->err == ERR_UNEXPECTED_EOF)
		rtn = ft_strdup("minishell: syntax error: unexpected end of file\n");
	if (p->err)
	{
		free(p);
		delete_ast_nodes(root, NULL);
		return (rtn);
	}
	return (NULL);
}

#else

bool	handle_err(t_parser *p, t_ast_node *root)
{
	if (p->err == ERR_UNEXPECTED_TOKEN)
	{
		write(STDERR_FILENO, "minishell: syntax error near unexpected token `", 48);
		if (p->token->type == NEWLINE_MS || p->token->type == SUBSHELL_NEWLINE_MS)
			write(STDERR_FILENO, "newline", 7);
		else
			write(STDERR_FILENO, p->token->literal.start, p->token->literal.len);
		write(STDERR_FILENO, "'\n", 2);
	}
	else if (p->err == ERR_UNEXPECTED_EOF)
		write(STDERR_FILENO, "minishell: syntax error: unexpected end of file\n", 48);
	else if (p->err == ERR_MALLOC)
	{
		perror("malloc");
		token_lstclear(p->token);
		free(p);
		delete_ast_nodes(root, NULL);
		exit(EXIT_FAILURE);
	}
	if (p->err)
	{
		//todo: change exit status `EXIT_STATUS_PARSER`
		return (true);
	}
	return (false);
}

#endif
