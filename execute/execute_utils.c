#include "execute.h"

bool	new_executor(t_executor **e, t_ast_node *root)
{
	*e = (t_executor *)malloc(sizeof(**e));
	if (!*e)
		return (false);
	(*e)->root = root;
	(*e)->exit_status = SUCCESS;
	(*e)->condition = CONDITION_AND_IF;
	return (true);
}
