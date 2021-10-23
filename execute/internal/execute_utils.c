#include "../../wrapper/x.h"
#include "../builtin/builtin.h"
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
