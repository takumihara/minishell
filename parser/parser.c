#include "parser.h"
#include <stdio.h>
#include <unistd.h>

static t_ast_node *command_line(t_parser *p);

//<command_line> '&&' <newline> <command_line> -> <pipeline> '&&' <newline> <command_line>
static t_ast_node *command_line1(t_parser *p);
//<command_line> '||' <newline> <command_line> -> <pipeline> '||' <newline> <command_line>
static t_ast_node *command_line2(t_parser *p);
//<pipeline>
static t_ast_node *command_line3(t_parser *p);

static t_ast_node *pipeline(t_parser *p);
//<pipeline> '|' <newline> <pipeline> -> <command> '|' <newline> <pipeline>
static t_ast_node *pipeline1(t_parser *p);
//<command>
static t_ast_node *pipeline2(t_parser *p);

//<simple_command>
//<subshell>
static t_ast_node *command(t_parser *p);

//'(' <compound_list> ')' <redirection_list>
//'(' <compound_list> ')'
static t_ast_node *subshell(t_parser *p);

static t_ast_node *compound_list(t_parser *p);
//<pipeline> '&&' <compound_list>
static t_ast_node *compound_list1(t_parser *p);
//<pipeline> '||' <compound_list>
static t_ast_node *compound_list2(t_parser *p);
//<pipeline> '\n' <compound_list>
static t_ast_node *compound_list3(t_parser *p);
//<pipeline>
static t_ast_node *compound_list4(t_parser *p);

static t_ast_node *simple_command(t_parser *p);
//<simple_command_element> <simple_command>
static t_ast_node *simple_command1(t_parser *p);
//<simple_command_element>
static t_ast_node *simple_command2(t_parser *p);

static t_ast_node *simple_command_element(t_parser *p);
//<word>
static t_ast_node *simple_command_element1(t_parser *p);
//<redirection>
static t_ast_node *simple_command_element2(t_parser *p);

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

//<command>		::= <simple_command>
//				|	<subshell>

//<subshell>	::=	'(' <compound_list> ')' <redirection_list>
//				|	'(' <compound_list> ')'

//<compound_list>	::=	<pipeline> '&&' <compound_list>
//           		|	<pipeline> '||' <compound_list>
//           		|	<pipeline> '\n' <compound_list>
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


t_ast_node *parse(t_token *token)
{
	t_parser		*p;
	t_ast_node		*root;

	if (!assign_mem((void **) &p, new_parser(token)))
		return (NULL);
	root = command_line(p);
	// todo: after EOL token was implemented, remove (p->token)
	if (p->token != NULL && p->token->type != EOL)
	{
		write(2, "Syntax Error near: ", 19);
		write(2, p->token->literal.start, p->token->literal.len);
		write(2, "\n", 1);
		return (NULL);
	}
	free(p);
	return (root);
}

// this works with norminette. what do you think?
t_ast_node	*command_line(t_parser *p)
{
	const t_token	*tmp = p->token;
	t_ast_node		*node;
	t_ast_node*		(*f[3])(t_parser*);
	int				i;

	f[0] = command_line1;
	f[1] = command_line2;
	f[2] = command_line3;
	i = -1;
	while (++i < 3)
	{
		if (assign_ast_node(&node, f[i](p)))
			return (node);
		p->token = (t_token *)tmp;
	}
	return (NULL);
}

t_ast_node *command_line1(t_parser *p)
{
	t_ast_node	*result;
	t_ast_node	*pipeline_;
	t_ast_node	*commandline_;

	pipeline_ = pipeline(p);
	if (!pipeline_)
		return (NULL);
	if (!consume_token(p, AND_IF, NULL))
		return (delete_ast_nodes(pipeline_, NULL));
	if (!assign_ast_node(&commandline_, command_line(p)))
		return (delete_ast_nodes(pipeline_, NULL));
	if (!new_ast_node(&result))
		return (delete_ast_nodes(pipeline_, commandline_));
	result->type = AND_IF_NODE;
	set_ast_nodes(result, pipeline_, commandline_);
	return (result);
}

t_ast_node *command_line2(t_parser *p)
{
	t_ast_node	*result;
	t_ast_node	*pipeline_;
	t_ast_node	*commandline_;

	pipeline_ = pipeline(p);
	if (!pipeline_)
		return (NULL);
	if (!consume_token(p, OR_IF, NULL))
		return (delete_ast_nodes(pipeline_, NULL));
	if (!assign_ast_node(&commandline_, command_line(p)))
		return (delete_ast_nodes(pipeline_, NULL));
	if (!new_ast_node(&result))
		return (delete_ast_nodes(pipeline_, commandline_));
	result->type = OR_IF_NODE;
	set_ast_nodes(result, pipeline_, commandline_);
	return (result);
}

t_ast_node *command_line3(t_parser *p)
{
	return pipeline(p);
}

t_ast_node *pipeline(t_parser *p)
{
	const t_token *tmp = p->token;
	t_ast_node *node;

	if (assign_ast_node(&node, pipeline1(p)))
		return (node);
	p->token = (t_token *)tmp;
	if (assign_ast_node(&node, pipeline2(p)))
		return (node);
	return (NULL);
}

t_ast_node *pipeline1(t_parser *p)
{
	t_ast_node	*result;
	t_ast_node	*command_;
	t_ast_node	*pipeline_;

	command_ = command(p);
	if (!command_)
		return (NULL);
	if (!consume_token(p, PIPE, NULL))
		return (delete_ast_nodes(command_, NULL));
	if (!assign_ast_node(&pipeline_, pipeline(p)))
		return (delete_ast_nodes(command_, NULL));
	if (!new_ast_node(&result))
		return (delete_ast_nodes(command_, pipeline_));
	result->type = PIPE_NODE;
	set_ast_nodes(result, command_, pipeline_);
	return (result);
}

t_ast_node *pipeline2(t_parser *p)
{
	return command(p);
}

t_ast_node *command(t_parser *p)
{
	const t_token	*tmp = p->token;
	t_ast_node		*node;

	if (assign_ast_node(&node, simple_command(p)))
		return (node);
	p->token = (t_token *)tmp;
	if (assign_ast_node(&node, subshell(p)))
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
	if (!assign_ast_node(&compound_list_, compound_list(p)))
		return (NULL);
	if (!consume_token(p, RPAREN, NULL))
		return (NULL);
	if (!new_ast_node(&result))
		return (delete_ast_nodes(compound_list_, NULL));
	result->type = SUBSHELL_NODE;
	redirection_list_ = redirection_list(p);
	// if redirection_list return NULL, NULL will be set for the right node
	set_ast_nodes(result, compound_list_, redirection_list_);
	return (result);
}

t_ast_node *compound_list(t_parser *p)
{
	t_token		*tmp;
	t_ast_node	*node;

	tmp = p->token;
	if (assign_ast_node(&node, compound_list1(p)))
		return (node);
	p->token = tmp;
	if (assign_ast_node(&node, compound_list2(p)))
		return (node);
	p->token = tmp;
	if (assign_ast_node(&node, compound_list3(p)))
		return (node);
	p->token = tmp;
	if (assign_ast_node(&node, compound_list4(p)))
		return (node);
	return (NULL);
}

t_ast_node *compound_list1(t_parser *p)
{
	t_ast_node	*result;
	t_ast_node	*pipeline_;
	t_ast_node	*compound_list_;

	if (!assign_ast_node(&pipeline_, pipeline(p)))
		return (NULL);
	if (!consume_token(p, AND_IF, NULL))
		return (delete_ast_nodes(pipeline_, NULL));
	if (!assign_ast_node(&compound_list_, compound_list(p)))
		return (delete_ast_nodes(pipeline_, NULL));
	if (!new_ast_node(&result))
		return (delete_ast_nodes(pipeline_, compound_list_));
	result->type = AND_IF_NODE;
	set_ast_nodes(result, pipeline_, compound_list_);
	return (result);
}

t_ast_node *compound_list2(t_parser *p)
{
	t_ast_node	*result;
	t_ast_node	*pipeline_;
	t_ast_node	*compound_list_;

	if (!assign_ast_node(&pipeline_, pipeline(p)))
		return (NULL);
	if (!consume_token(p, OR_IF, NULL))
		return (delete_ast_nodes(pipeline_, NULL));
	if (!assign_ast_node(&compound_list_, compound_list(p)))
		return (delete_ast_nodes(pipeline_, NULL));
	if (!new_ast_node(&result))
		return (delete_ast_nodes(pipeline_, compound_list_));
	result->type = OR_IF_NODE;
	set_ast_nodes(result, pipeline_, compound_list_);
	return (result);
}

t_ast_node *compound_list3(t_parser *p)
{
	t_ast_node	*result;
	t_ast_node	*pipeline_;
	t_ast_node	*compound_list_;

	if (!assign_ast_node(&pipeline_, pipeline(p)))
		return (NULL);
	// todo: change OR_IF to newly created token type name
	if (!consume_token(p, OR_IF, NULL))
		return (delete_ast_nodes(pipeline_, NULL));
	if (!assign_ast_node(&compound_list_, compound_list(p)))
		return (delete_ast_nodes(pipeline_, NULL));
	if (!new_ast_node(&result))
		return (delete_ast_nodes(pipeline_, compound_list_));
	result->type = SUBSHELL_NEWLINE_NODE;
	set_ast_nodes(result, pipeline_, compound_list_);
	return (result);
}

t_ast_node *compound_list4(t_parser *p)
{
	return (pipeline(p));
}

t_ast_node *simple_command(t_parser *p)
{
	const t_token	*tmp = p->token;
	t_ast_node		*node;

	if (assign_ast_node(&node, simple_command1(p)))
		return (node);
	p->token = (t_token *)tmp;
	if (assign_ast_node(&node, simple_command2(p)))
		return (node);
	return (NULL);
}

t_ast_node *simple_command1(t_parser *p)
{
	t_ast_node	*simple_command_;
	t_ast_node	*simple_command_element_;

	simple_command_element_ = simple_command_element(p);
	if (!simple_command_element_)
		return (NULL);
	simple_command_ = command(p);
	if (!simple_command_)
		return (delete_ast_nodes(simple_command_, NULL));
	attach_ast_nodes(simple_command_element_, NULL, simple_command_);
	return (simple_command_element_);
}

t_ast_node *simple_command2(t_parser *p)
{
	return (simple_command_element(p));
}

t_ast_node *simple_command_element(t_parser *p)
{
	const t_token	*tmp = p->token;
	t_ast_node		*node;

	if (assign_ast_node(&node, simple_command_element1(p)))
		return (node);
	p->token = (t_token *)tmp;
	if (assign_ast_node(&node, simple_command_element2(p)))
		return (node);
	return (NULL);
}

t_ast_node *simple_command_element1(t_parser *p)
{
	t_ast_node	*simple_command_element;

	if (!new_ast_node(&simple_command_element))
		return (NULL);
	if (!consume_token(p, STRING, simple_command_element))
		return (delete_ast_nodes(simple_command_element, NULL));
	simple_command_element->type = COMMAND_ARG_NODE;
	set_ast_nodes(simple_command_element, NULL, NULL);
	return (simple_command_element);
}

t_ast_node *simple_command_element2(t_parser *p)
{
	return redirection(p);
}

t_ast_node *redirection_list(t_parser *p)
{
	t_ast_node *redirection_;
	t_ast_node *redirection_list_;

	if (!assign_ast_node(&redirection_, redirection(p)))
		return (NULL);
	if (assign_ast_node(&redirection_list_, redirection_list(p)))
		attach_ast_nodes(redirection_, NULL, redirection_list_);
	return (redirection_);
}

t_ast_node *redirection(t_parser *p)
{
	t_ast_node	*redirection;
	t_ast_node	*operand;

	if (!new_ast_node(&redirection))
		return (NULL);
	consume_token(p, REDIRECT_MODIFIER, redirection);
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
		return (delete_ast_nodes(redirection, operand));
	operand->type = REDIRECT_OPERAND_NODE;
	set_ast_nodes(redirection, operand, NULL);
	return (redirection);
}
