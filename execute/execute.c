#include "../ast/ast.h"
#include "../env/env.h"
#include "internal/execute_internal.h"
#include "exit_status.h"
#include "../signal/signal.h"

int	execute(t_ast_node *root, t_env_var **env_vars, bool is_interactive)
{
	t_executor	*e;
	int			exit_status;

	set_signal_handler(EXECUTE_SIGNAL);
	if (!root)
	{
		register_env_var_from_literal("?", NULL, ES_SYNTAX_ERROR, env_vars);
		return (ES_SYNTAX_ERROR);
	}
	new_executor(&e, env_vars, root, is_interactive);
	exit_status = execute_command_line(e, root);
	delete_ast_nodes(root, NULL);
	free(e);
	return (exit_status);
}
