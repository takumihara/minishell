#include "parser.h"
#include <stdio.h>

//<pipeline> '&&' <newline> <command_line>
//<pipeline> '||' <newline> <command_line>
//<pipeline>
static t_ast_node *command_line(t_parser *p);

//<command> '|' <newline> <pipeline>
//<command>
static t_ast_node *pipeline(t_parser *p);

//<subshell>
//<simple_command>
static t_ast_node *command(t_parser *p);

//'(' <compound_list> ')' <redirection_list>
//'(' <compound_list> ')'
static t_ast_node *subshell(t_parser *p);

//<pipeline> '&&' <compound_list>
//<pipeline> '||' <compound_list>
//<pipeline> '\n' <compound_list>
//<pipeline>
static t_ast_node *compound_list(t_parser *p);

//<simple_command_element> <simple_command>
//<simple_command_element>
static t_ast_node *simple_command(t_parser *p);

//<word>
//<redirection>
static t_ast_node *simple_command_element(t_parser *p);
static t_ast_node *word(t_parser *p);

//<redirection> <redirection_list>
//<redirection>
static t_ast_node *redirection_list(t_parser *p);

//<number>? '>' <word>
//<number>? '<' <word>
//<number>?　'>>' <word>
//<number>? '<<' <word>
static t_ast_node *redirection(t_parser *p);


// originally named 'simple_list'
//<command_line>	::= <pipeline> '&&' <newline> <command_line>
//					|   <pipeline> '||' <newline> <command_line>
//					|   <pipeline>

//<pipeline>	::=  <command> '|' <newline> <pipeline>
//				|   <command>

//<command>		::= <subshell>
//				|	<simple_command>

//<subshell>	::=	'(' <compound_list> ')' <redirection_list>
//				|	'(' <compound_list> ')'

//<compound_list>	::=	<pipeline> '&&' <newline> <compound_list>
//           		|	<pipeline> '||' <newline> <compound_list>
//           		|	<pipeline> '\n' <newline> <compound_list>
//           		|	<pipeline>

//<simple_command>	::= <simple_command_element> <simple_command>
//					|   <simple_command_element>

//<simple_command_element>	::= <word>
//							|	<redirection>

//<redirection_list> ::= <redirection> <redirection_list>
//                    |  <redirection>

//<redirection>	::= <number>? '>' <word>
//				|	<number>? '<' <word>
//				|	<number>? '>>' <word>
//				|	<number>? '<<' <word>

#ifdef TEST

void	*parse(t_token *token)
{
	t_parser	*p;
	t_ast_node	*node;
	char		*err_msg;

	if (!assign_mem((void **) &p, new_parser(token)))
		return (NULL);
	node = command_line(p);
	err_msg = handle_err(p);
	if (err_msg)
		return ((void *)err_msg);
	free(p);
	return ((void *)node);
}

#else

t_ast_node *parse(t_token *token)
{
	t_parser		*p;
	t_ast_node		*root;

	if (!assign_mem((void **) &p, new_parser(token)))
		return (NULL);
	root = command_line(p);
	handle_err(p);
	free(p);
	return (root);
}

#endif

t_ast_node	*command_line(t_parser *p)
{
	t_ast_node	*result;
	t_ast_node	*pipeline_;
	t_ast_node	*commandline_;

	if (!assign_ast_node(&pipeline_, pipeline(p)))
		return (NULL);
	if (!new_ast_node(&result))
		return (delete_ast_nodes(pipeline_, NULL));
	if (consume_token(p, AND_IF, NULL))
		result->type = AND_IF_NODE;
	else if (consume_token(p, OR_IF, NULL))
		result->type = OR_IF_NODE;
	else
	{
		free(result);
		return (pipeline_);
	}
	if (!assign_ast_node(&commandline_, command_line(p)))
	{
		p->err = ERR_UNEXPECTED_EOF;
		return (delete_ast_nodes(pipeline_, result));
	}
	set_ast_nodes(result, pipeline_, commandline_);
	return (result);
}

t_ast_node *pipeline(t_parser *p)
{
	t_ast_node	*result;
	t_ast_node	*command_;
	t_ast_node	*pipeline_;

	if (!assign_ast_node(&command_, command(p)))
		return (NULL);
	if (!consume_token(p, PIPE, NULL))
		return (command_);
	if (!assign_ast_node(&pipeline_, pipeline(p)))
	{
		p->err = ERR_UNEXPECTED_EOF;
		return (delete_ast_nodes(command_, NULL));
	}
	if (!new_ast_node(&result))
		return (delete_ast_nodes(command_, pipeline_));
	result->type = PIPE_NODE;
	set_ast_nodes(result, command_, pipeline_);
	return (result);
}

t_ast_node *command(t_parser *p)
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

t_ast_node *subshell(t_parser *p)
{
	t_ast_node	*result;
	t_ast_node	*compound_list_;
	t_ast_node	*redirection_list_;

	if (!consume_token(p, LPAREN, NULL))
		return (NULL);
	p->is_subshell = true;
	consume_token(p, SUBSHELL_NEWLINE, NULL);
	if (!assign_ast_node(&compound_list_, compound_list(p)))
	{
		if (!p->err)
			p->err = ERR_UNEXPECTED_TOKEN;
		return (NULL);
	}
	if (!consume_token(p, RPAREN, NULL))
	{
		p->err = ERR_UNEXPECTED_EOF;
		return (delete_ast_nodes(compound_list_, NULL));
	}
	p->is_subshell = false;
	if (!new_ast_node(&result))
		return (delete_ast_nodes(compound_list_, NULL));
	result->type = SUBSHELL_NODE;
	redirection_list_ = redirection_list(p);
	if (p->err)
		return (delete_ast_nodes(result, compound_list_));
	set_ast_nodes(result, compound_list_, redirection_list_);
	return (result);
}

t_ast_node *compound_list(t_parser *p)
{
	t_ast_node	*result;
	t_ast_node	*pipeline_;
	t_ast_node	*compound_list_;

	if (!assign_ast_node(&pipeline_, pipeline(p)))
		return (NULL);
	if (!new_ast_node(&result))
		return (delete_ast_nodes(pipeline_, NULL));
	if (consume_token(p, AND_IF, NULL))
		result->type = AND_IF_NODE;
	else if (consume_token(p, OR_IF, NULL))
		result->type = OR_IF_NODE;
	else if (consume_token(p, SUBSHELL_NEWLINE, NULL))
		result->type = SUBSHELL_NEWLINE_NODE;
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

t_ast_node *simple_command(t_parser *p)
{
	t_ast_node	*simple_command_;
	t_ast_node	*simple_command_element_;

	if (!assign_ast_node(&simple_command_element_, simple_command_element(p)))
		return (NULL);
	if (assign_ast_node(&simple_command_, simple_command(p)))
		attach_ast_nodes(simple_command_element_, NULL, simple_command_);
	if (p->err)
		return (delete_ast_nodes(simple_command_element_, NULL));
	return (simple_command_element_);
}

t_ast_node *simple_command_element(t_parser *p)
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

t_ast_node *word(t_parser *p)
{
	t_ast_node		*simple_command_element;

	if (!new_ast_node(&simple_command_element))
		return (NULL);
	if (!consume_token(p, STRING, simple_command_element))
	{
		if (!p->err && (p->token->type == LPAREN || (p->token->type == RPAREN && !p->is_subshell)))
			p->err = ERR_UNEXPECTED_TOKEN;
		return (delete_ast_nodes(simple_command_element, NULL));
	}
	simple_command_element->type = COMMAND_ARG_NODE;
	return (simple_command_element);
}

t_ast_node *redirection_list(t_parser *p)
{
	t_ast_node *redirection_;
	t_ast_node *redirection_list_;

	if (!assign_ast_node(&redirection_, redirection(p)))
		return (NULL);
	if (assign_ast_node(&redirection_list_, redirection_list(p)))
		attach_ast_nodes(redirection_, NULL, redirection_list_);
	if (p->err)
		return (delete_ast_nodes(redirection_, NULL));
	return (redirection_);
}

t_ast_node *redirection(t_parser *p)
{
	t_ast_node	*redirection;
	t_ast_node	*operand;

	if (!new_ast_node(&redirection))
		return (NULL);
	consume_token(p, REDIRECT_MODIFIER, redirection);
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
	if (!new_ast_node(&operand))
		return (delete_ast_nodes(redirection, NULL));
	if (!consume_token(p, STRING, operand))
	{
		if (!p->err)
			p->err = ERR_UNEXPECTED_TOKEN;
		return (delete_ast_nodes(redirection, operand));
	}
	operand->type = REDIRECT_OPERAND_NODE;
	set_ast_nodes(redirection, operand, NULL);
	return (redirection);
}
