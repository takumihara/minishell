#include "parser_internal.h"

//<simple_command_element> <simple_command>
//<simple_command_element>
t_ast_node			*simple_command(t_parser *p);

//<word>
//<redirection>
static t_ast_node	*simple_command_element(t_parser *p);
static t_ast_node	*word(t_parser *p);

//<number>? '>' <word>
//<number>? '<' <word>
//<number>?　'>>' <word>
//<number>? '<<' <word>
static t_ast_node	*redirection(t_parser *p);

t_ast_node	*simple_command(t_parser *p)
{
	t_ast_node	*simple_command_;
	t_ast_node	*simple_command_element_;

	if (!assign_ast_node(&simple_command_element_, simple_command_element(p)))
		return (NULL);
	p->is_first_word = false;
	if (assign_ast_node(&simple_command_, simple_command(p)))
		attach_ast_nodes(simple_command_element_, NULL, simple_command_);
	if (p->err)
		return (delete_ast_nodes(simple_command_element_, NULL));
	return (simple_command_element_);
}

t_ast_node	*simple_command_element(t_parser *p)
{
	const t_token	*tmp = p->token;
	t_ast_node		*node;

	if (assign_ast_node(&node, word(p)))
		return (node);
	if (p->err)
		return (NULL);
	p->token = (t_token *)tmp;
	if (assign_ast_node(&node, redirection(p)))
		return (node);
	return (NULL);
}

t_ast_node	*word(t_parser *p)
{
	t_ast_node		*simple_command_element;

	new_ast_node(&simple_command_element);
	if (!consume_token(p, STRING, simple_command_element))
	{
		if ((p->is_first_word
				&& (p->token->type == AND_IF
					|| p->token->type == OR_IF
					|| p->token->type == PIPE
					|| p->token->type == LPAREN
					|| p->token->type == RPAREN))
			|| (!p->is_subshell && p->token->type == RPAREN))
			p->err = ERR_UNEXPECTED_TOKEN;
		return (delete_ast_nodes(simple_command_element, NULL));
	}
	simple_command_element->type = COMMAND_ARG_NODE;
	return (simple_command_element);
}

t_ast_node	*redirection(t_parser *p)
{
	t_ast_node	*redirection;

	new_ast_node(&redirection);
	if (p->err)
		return (NULL);
	if (consume_token(p, REDIRECT_IN, NULL))
		redirection->type = REDIRECT_IN_NODE;
	else if (consume_token(p, REDIRECT_OUT, NULL))
		redirection->type = REDIRECT_OUT_NODE;
	else if (consume_token(p, REDIRECT_APPEND, NULL))
		redirection->type = REDIRECT_APPEND_NODE;
	else if (consume_token(p, HEREDOC, NULL))
		redirection->type = HEREDOC_NODE;
	else
		return (delete_ast_nodes(redirection, NULL));
	if (!consume_token(p, STRING, redirection))
	{
		if (!p->err)
			p->err = ERR_UNEXPECTED_TOKEN;
		return (delete_ast_nodes(redirection, NULL));
	}
	return (redirection);
}
