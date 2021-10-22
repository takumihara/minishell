#include "../ast/ast.h"
#include "../env/env.h"
#include "internal/execute_internal.h"
#include "internal/exit_status.h"

int	execute(t_ast_node *root, t_env_var **env_vars)
{
	t_executor	*e;
	int			exit_status;

	if (!root)
	{
		register_env_var_from_literal("?", NULL, ES_SYNTAX_ERROR, env_vars);
		return (ES_SYNTAX_ERROR);
	}
	new_executor(&e, env_vars);
	exit_status = init_command_line(e, root);
	delete_ast_nodes(root, NULL);
	free(e);
	return (exit_status);
}
