#include "internal/execute_internal.h"
#include "execute.h"

void	delete_execute_list(void *element, t_execute_list_type type)
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
