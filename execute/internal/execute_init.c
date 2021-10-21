#include "../execute.h"
#include "exit_status.h"
#include "execute_internal.h"
#include "../../expander/expander.h"

static void	init_pipeline(t_executor *e,
				 t_pipeline **pipeline, t_ast_node *node);
static void	init_subshell(t_executor *e, t_subshell **ss, t_ast_node *node);
static void	init_simple_command(t_executor *e,
				   t_simple_command **sc, t_ast_node *node);

int	init_command_line(t_executor *e, t_ast_node *node)
{
	t_ast_node	*pipeline_node;

	if (node->type == AND_IF_NODE || node->type == OR_IF_NODE)
		pipeline_node = node->left;
	else
		pipeline_node = node;
	if (is_execute_condition(e->condition, e->exit_status))
	{
		init_pipeline(e, &e->pipeline, pipeline_node);
		execute_pipeline(e, e->pipeline);
		register_env_var_from_literal("?", NULL, e->exit_status, e->env_vars);
		delete_list(e->pipeline, T_PIPELINE);
	}
	if (node->type == AND_IF_NODE || node->type == OR_IF_NODE)
	{
		if (node->type == AND_IF_NODE)
			e->condition = CONDITION_AND_IF;
		else if (node->type == OR_IF_NODE)
			e->condition = CONDITION_OR_IF;
		return (init_command_line(e, node->right));
	}
	return (e->exit_status);
}

// expected node: COMMAND_ARG_NODE, REDIRECT*, PIPE_NODE, SUBSHELL
void	init_pipeline(t_executor *e, t_pipeline **pipeline_, t_ast_node *node)
{
	t_pipeline	*pipeline_next;

	pipeline_next = NULL;
	new_t_pipeline(pipeline_);
	if (node->type == PIPE_NODE)
	{
		init_pipeline(e, &pipeline_next, node->right);
		node = node->left;
	}
	if (node->type == SUBSHELL_NODE)
	{
		init_subshell(e, (t_subshell **)&(*pipeline_)->command, node);
		(*pipeline_)->type = T_SUBSHELL;
	}
	else
	{
		init_simple_command(e,
			(t_simple_command **)&(*pipeline_)->command, node);
		(*pipeline_)->type = T_SIMPLE_COMMAND;
	}
	if (pipeline_next)
		(*pipeline_)->next = pipeline_next;
}

void	init_subshell(t_executor *e, t_subshell **ss, t_ast_node *node)
{
	new_t_subshell(ss);
	init_compound_list(e, &(*ss)->compound_list, node->left);
}

void	init_compound_list(t_executor *e,
			   t_compound_list **cl, t_ast_node *node)
{
	new_t_compound_list(cl);
	if (node->type == AND_IF_NODE
		|| node->type == OR_IF_NODE
		|| node->type == SUBSHELL_NEWLINE_MS_NODE)
	{
		init_pipeline(e, &(*cl)->pipeline, node->left);
		(*cl)->compound_list_next = node->right;
		if (node->type == AND_IF_NODE)
			(*cl)->condition = CONDITION_AND_IF;
		else if (node->type == OR_IF_NODE)
			(*cl)->condition = CONDITION_OR_IF;
		else if (node->type == SUBSHELL_NEWLINE_MS_NODE)
			(*cl)->condition = CONDITION_NL;
	}
	else
		init_pipeline(e, &(*cl)->pipeline, node);
}

// expected node: COMMAND_ARG_NODE, REDIRECT*
void	init_simple_command(t_executor *e,
				t_simple_command **sc, t_ast_node *node)
{
	new_t_simple_command(sc);
	(*sc)->root = node;
	if (!expand(node, e->env_vars))
	{
		(*sc)->err = EXPANSION_ERR;
		return ;
	}
	while (node != NULL)
	{
		if (node->type == COMMAND_ARG_NODE)
			(*sc)->argc++;
		else if (node->type == REDIRECT_OUT_NODE
			|| node->type == REDIRECT_APPEND_NODE)
			new_redirect_out(*sc, node->data, node->type);
		else if (node->type == REDIRECT_IN_NODE || node->type == HEREDOC_NODE)
			new_redirect_in(*sc, node->data, node->type);
		node = node->right;
	}
	new_argv(*sc);
}
