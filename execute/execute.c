#include "execute.h"

static int command_line(t_executor *e, t_ast_node *node);
static int pipeline(t_executor *e, t_ast_node *node);
static int command(t_executor *e, t_ast_node *node);
static int subshell(t_executor *e, t_ast_node *node);
static int compound_list(t_executor *e, t_ast_node *node);
static int simple_command(t_executor *e, t_ast_node *node);
static int simple_command_element(t_executor *e, t_ast_node *node);
static int word(t_executor *e, t_ast_node *node);
static int redirection_list(t_executor *e, t_ast_node *node);
static int redirection(t_executor *e, t_ast_node *node);

void	execute(t_ast_node *root)
{
	t_executor	*e;

	if (new_executor(&e, root))
		return ;
	command_line(e, root);
}

int command_line(t_executor *e, t_ast_node *node)
{
	if (e->exit_status == e->condition)
		e->exit_status = pipeline(e, node->left);
	if (node->type == AND_IF_NODE)
	{
		e->condition = CONDITION_AND_IF;
		return (command_line(e, node->right));
	}
	else if (node->type == OR_IF_NODE)
	{
		e->condition = CONDITION_OR_IF;
		return (command_line(e, node->right));
	}
	return (e->exit_status);
}

int pipeline(t_executor *e, t_ast_node *node)
{
	// expected node type == COMMAND_ARG_NODE, REDIRECT*, PIPE_NODE, SUBSHELL
	if (node->type != PIPE_NODE)
		return (command(e, node));

}

int command(t_executor *e, t_ast_node *node)
{
	if (node->type == SUBSHELL_NODE)
		return (subshell(e, node));
	else
		return (simple_command(e, node));
}

int subshell(t_executor *e, t_ast_node *node)
{

}
