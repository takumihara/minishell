#include "parser_internal.h"

//<pipeline> '&&' <newline> <command_line>
//<pipeline> '||' <newline> <command_line>
//<pipeline>
t_ast_node			*command_line(t_parser *p);

//<command> '|' <newline> <pipeline>
//<command>
static t_ast_node	*pipeline(t_parser *p);

//<subshell>
//<simple_command>
static t_ast_node	*command(t_parser *p);

//'(' <compound_list> ')' <redirection_list>
//'(' <compound_list> ')'
static t_ast_node	*subshell(t_parser *p);

//<pipeline> '&&' <compound_list>
//<pipeline> '||' <compound_list>
//<pipeline> '\n' <compound_list>
//<pipeline>
static t_ast_node	*compound_list(t_parser *p);

t_ast_node	*command_line(t_parser *p)
{
	t_ast_node	*result;
	t_ast_node	*pipeline_;
	t_ast_node	*commandline_;

	if (!assign_ast_node(&pipeline_, pipeline(p)))
		return (NULL);
	if (p->token->type == EOL)
		return (pipeline_);
	new_ast_node(&result);
	if (consume_token(p, AND_IF, NULL))
		result->type = AND_IF_NODE;
	else if (consume_token(p, OR_IF, NULL))
		result->type = OR_IF_NODE;
	else
	{
		p->err = ERR_UNEXPECTED_TOKEN;
		return (delete_ast_nodes(pipeline_, result));
	}
	if (!assign_ast_node(&commandline_, command_line(p)))
	{
		p->err = ERR_UNEXPECTED_EOF;
		return (delete_ast_nodes(pipeline_, result));
	}
	set_ast_nodes(result, pipeline_, commandline_);
	return (result);
}

t_ast_node	*pipeline(t_parser *p)
{
	t_ast_node	*result;
	t_ast_node	*command_;
	t_ast_node	*pipeline_;

	p->is_first_word = true;
	if (!assign_ast_node(&command_, command(p)))
		return (NULL);
	if (!consume_token(p, PIPE, NULL))
		return (command_);
	if (!assign_ast_node(&pipeline_, pipeline(p)))
	{
		if (!p->err)
			p->err = ERR_UNEXPECTED_EOF;
		return (delete_ast_nodes(command_, NULL));
	}
	new_ast_node(&result);
	result->type = PIPE_NODE;
	set_ast_nodes(result, command_, pipeline_);
	return (result);
}

t_ast_node	*command(t_parser *p)
{
	const t_token	*tmp = p->token;
	t_ast_node		*node;

	if (assign_ast_node(&node, subshell(p)))
		return (node);
	if (p->err)
		return (NULL);
	p->token = (t_token *)tmp;
	if (assign_ast_node(&node, simple_command(p)))
		return (node);
	return (NULL);
}

t_ast_node	*subshell(t_parser *p)
{
	t_ast_node	*result;
	t_ast_node	*compound_list_;
	static int	subshell_level;

	if (!consume_token(p, LPAREN, NULL))
		return (NULL);
	subshell_level++;
	p->is_subshell = true;
	consume_token(p, SUBSHELL_NEWLINE_MS, NULL);
	if (!assign_ast_node(&compound_list_, compound_list(p)))
	{
		!p->err && (p->err = ERR_UNEXPECTED_EOF);
		return (NULL);
	}
	if (!consume_token(p, RPAREN, NULL))
	{
		p->err = ERR_UNEXPECTED_EOF;
		return (delete_ast_nodes(compound_list_, NULL));
	}
	if (--subshell_level == 0)
		p->is_subshell = false;
	new_ast_node(&result);
	result->type = SUBSHELL_NODE;
	set_ast_nodes(result, compound_list_, NULL);
	return (result);
}

t_ast_node	*compound_list(t_parser *p)
{
	t_ast_node	*result;
	t_ast_node	*pipeline_;
	t_ast_node	*compound_list_;

	if (!assign_ast_node(&pipeline_, pipeline(p)))
		return (NULL);
	new_ast_node(&result);
	if (consume_token(p, AND_IF, NULL))
		result->type = AND_IF_NODE;
	else if (consume_token(p, OR_IF, NULL))
		result->type = OR_IF_NODE;
	else if (consume_token(p, SUBSHELL_NEWLINE_MS, NULL))
		result->type = SUBSHELL_NEWLINE_MS_NODE;
	else
	{
		free(result);
		return (pipeline_);
	}
	if (!assign_ast_node(&compound_list_, compound_list(p)))
	{
		p->err = ERR_UNEXPECTED_EOF;
		return (delete_ast_nodes(pipeline_, result));
	}
	set_ast_nodes(result, pipeline_, compound_list_);
	return (result);
}
