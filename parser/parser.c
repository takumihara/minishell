#include "parser.h"
#include <stdio.h>
#include <unistd.h>

// originally named 'simple_list'
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

//static t_ast_node *newline(t_parser *p);
//static t_ast_node *newline1(t_parser *p);
//static t_ast_node *newline2(t_parser *p);

static t_ast_node *command(t_parser *p);
//<command_element> <command>
static t_ast_node *command1(t_parser *p);
//<command_element>
static t_ast_node *command2(t_parser *p);

static t_ast_node *command_element(t_parser *p);
//<word>
static t_ast_node *command_element1(t_parser *p);
//<redirection>
static t_ast_node *command_element2(t_parser *p);

//<number>? '>' <word>
//<number>? '<' <word>
//<number>?　'>>' <word>
//<number>? '<<' <word>
static t_ast_node *redirection(t_parser *p);

//<command_line>	::= <pipeline> '&&' <newline> <command_line>
//					|   <pipeline> '||' <newline> <command_line>
//					|   <pipeline>

//<pipeline>	::=  <command> '|' <newline> <pipeline>
//				|   <command>

// originally 'newline_list'
//<newline>	::=  <newline> '\n'
//			|   (empty)

//<command>			::= <command_element> <command>
//					|   <command_element>
//<command_element>	::= <word>
//					|	<redirection>

//<redirection>	::= <number>? '>' <word>
//				|	<number>? '<' <word>
//				|	<number>? '>>' <word>
//				|	<number>? '<<' <word>


t_ast_node *parse(t_token *token)
{
	t_parser		*p;
	t_ast_node		*root;

	p = new_parser(token);
	if (!p)
		return (NULL);
	// todo: after EOL token was implemented, remove (p->token)
	root = command_line(p);
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

t_ast_node *command_line(t_parser *p)
{
	t_token		*tmp;
	t_ast_node 	*node;

	tmp = p->token;
	node = command_line1(p);
	if (node)
		return (node);
	p->token = tmp;
	node = command_line2(p);
	if (node)
		return (node);
	p->token = tmp;
	node = command_line3(p);
	if (node)
		return (node);
	return (NULL);
}

t_ast_node *command_line1(t_parser *p)
{
	t_ast_node	*result;
	t_ast_node	*pipeline1;
	t_ast_node	*commandline1;

	pipeline1 = pipeline(p);
	if (!pipeline1)
		return (NULL);
	if (!consume_token(p, AND_IF, NULL))
		return (delete_ast_nodes(pipeline1, NULL));
	commandline1 = command_line(p);
	// todo: newline
	if (!commandline1)
		return (delete_ast_nodes(pipeline1, NULL));
	if (!new_ast_node(&result))
		return (delete_ast_nodes(pipeline1, commandline1));
	result->type = AND_IF_NODE;
	set_ast_nodes(result, pipeline1, commandline1);
	return (result);
}

t_ast_node *command_line2(t_parser *p)
{
	t_ast_node	*result;
	t_ast_node	*pipeline1;
	t_ast_node	*commandline1;

	pipeline1 = pipeline(p);
	if (!pipeline1)
		return (NULL);
	if (!consume_token(p, OR_IF, NULL))
		return (delete_ast_nodes(pipeline1, NULL));
	commandline1 = command_line(p);
	// todo: newline
	if (!commandline1)
		return (delete_ast_nodes(pipeline1, NULL));
	if (!new_ast_node(&result))
		return (delete_ast_nodes(pipeline1, commandline1));
	result->type = OR_IF_NODE;
	set_ast_nodes(result, pipeline1, commandline1);
	return (result);
}

t_ast_node *command_line3(t_parser *p)
{
	return pipeline(p);
}

t_ast_node *pipeline(t_parser *p)
{
	t_token *tmp;
	t_ast_node *node;

	tmp = p->token;
	node = pipeline1(p);
	if (node)
		return (node);
	p->token = tmp;
	node = pipeline2(p);
	if (node)
		return (node);
	return (NULL);
}

t_ast_node *pipeline1(t_parser *p)
{
	t_ast_node	*result;
	t_ast_node	*command1;
	t_ast_node	*pipeline1;

	command1 = command(p);
	if (!command1)
		return (NULL);
	if (!consume_token(p, PIPE, NULL))
		return (delete_ast_nodes(command1, NULL));
	pipeline1 = pipeline(p);
	// todo: newline
	if (!pipeline1)
		return (delete_ast_nodes(command1, NULL));
	if (!new_ast_node(&result))
		return (delete_ast_nodes(command1, pipeline1));
	result->type = PIPE_NODE;
	set_ast_nodes(result, command1, pipeline1);
	return (result);
}

t_ast_node *pipeline2(t_parser *p)
{
	return command(p);
}

t_ast_node *command(t_parser *p)
{
	t_token *tmp;
	t_ast_node *node;

	tmp = p->token;
	node = command1(p);
	if (node)
		return (node);
	p->token = tmp;
	node = command2(p);
	if (node)
		return (node);
	return (NULL);
}

t_ast_node *command1(t_parser *p)
{
	t_ast_node	*command1;
	t_ast_node	*command_element1;

	command_element1 = command_element(p);
	if (!command_element1)
		return (NULL);
	command1 = command(p);
	if (!command1)
		return (delete_ast_nodes(command_element1, NULL));
	set_ast_nodes(command_element1, NULL, command1);
	return (command_element1);
}

t_ast_node *command2(t_parser *p)
{
	return (command_element(p));
}

t_ast_node *command_element(t_parser *p)
{
	t_token *tmp;
	t_ast_node *node;

	tmp = p->token;
	node = command_element1(p);
	if (node)
		return (node);
	p->token = tmp;
	node = command_element2(p);
	if (node)
		return (node);
	return (NULL);
}

t_ast_node *command_element1(t_parser *p)
{
	t_ast_node	*command_element;

	if (!new_ast_node(&command_element))
		return (NULL);
	if (!consume_token(p, STRING, command_element))
	{
		free(command_element);
		return (NULL);
	}
	command_element->type = COMMAND_ARG_NODE;
	set_ast_nodes(command_element, NULL, NULL);
	return (command_element);
}

t_ast_node *command_element2(t_parser *p)
{
	return redirection(p);
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
	set_ast_nodes(redirection, NULL, operand);
	return (redirection);
}
