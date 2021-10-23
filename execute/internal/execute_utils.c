#include "../../wrapper/x.h"
#include "../../builtin/builtin.h"
#include "execute_internal.h"

void	new_executor(t_executor **e, t_env_var **env_vars, t_ast_node *root, bool is_interactive)
{
	*e = x_malloc(sizeof(**e));
	(*e)->exit_status = EXIT_SUCCESS;
	(*e)->condition = CONDITION_AND_IF;
	(*e)->pipeline = NULL;
	(*e)->root = root;
	(*e)->env_vars = env_vars;
	(*e)->is_interactive = is_interactive;
}

void	delete_execute_list(void *element, t_list_type type)
{
	if (!element)
		return ;
	else if (type == T_SIMPLE_COMMAND)
	{
		free(((t_simple_command *)element)->argv);
		close(((t_simple_command *)element)->r_out);
		close(((t_simple_command *)element)->r_in);
	}
	else if (type == T_COMPOUND_LIST)
	{
		delete_execute_list(((t_compound_list *)element)->pipeline, T_PIPELINE);
		delete_execute_list(((t_compound_list *)element)->next, T_COMPOUND_LIST);
	}
	else if (type == T_SUBSHELL)
		delete_execute_list(((t_subshell *)element)->compound_list, T_COMPOUND_LIST);
	else if (type == T_PIPELINE)
	{
		delete_execute_list(((t_pipeline *)element)->command,
			((t_pipeline *)element)->type);
		delete_execute_list(((t_pipeline *)element)->next, T_PIPELINE);
	}
	free(element);
}

bool	is_execute_condition(int condition, int exit_status)
{
	if (condition == CONDITION_AND_IF && exit_status == EXIT_SUCCESS)
		return (true);
	if (condition == CONDITION_OR_IF && exit_status != EXIT_SUCCESS)
		return (true);
	if (condition == CONDITION_NL)
		return (true);
	return (false);
}
