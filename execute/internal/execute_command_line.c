#include "execute_internal.h"
#include "eval.h"

int	execute_command_line(t_executor *e, t_ast_node *node)
{
	t_ast_node	*pipeline_node;

	if (node->type == AND_IF_NODE || node->type == OR_IF_NODE)
		pipeline_node = node->left;
	else
		pipeline_node = node;
	if (is_execute_condition(e->condition, e->exit_status))
	{
		eval_pipeline(e, &e->pipeline, pipeline_node);
		execute_pipeline(e, e->pipeline);
		register_env_var_from_literal("?", NULL, e->exit_status, e->env_vars);
		delete_execute_list(e->pipeline, T_PIPELINE);
	}
	if (node->type == AND_IF_NODE || node->type == OR_IF_NODE)
	{
		if (node->type == AND_IF_NODE)
			e->condition = CONDITION_AND_IF;
		else if (node->type == OR_IF_NODE)
			e->condition = CONDITION_OR_IF;
		return (execute_command_line(e, node->right));
	}
	return (e->exit_status);
}
