#include "execute_internal.h"
#include "eval.h"

static int	execute_subshell(t_executor *e, t_subshell *ss);
static int	execute_compound_list(t_executor *e, t_compound_list *cl);

int	execute_command(t_executor *e,
					   void *command, int type, t_exec_pipe_info *info)
{
	if (type == T_SIMPLE_COMMAND)
	{
		if (((t_simple_command *)command)->err != NO_SC_ERR)
		{
			e->exit_status = EXIT_FAILURE;
			return (CHILD_PROCESS_NOT_CREATED);
		}
		return (execute_simple_command(e, (t_simple_command *)command, info));
	}
	else
		return (execute_subshell(e, (t_subshell *)command));
}

int	execute_subshell(t_executor *e, t_subshell *ss)
{
	return (execute_compound_list(e, ss->compound_list));
}

int	execute_compound_list(t_executor *e, t_compound_list *cl)
{
	pid_t			pid;
	t_executor		*exe_child;
	int				exit_status;

	pid = x_fork();
	if (pid == CHILD_PROCESS)
	{
		new_executor(&exe_child, NULL);
		exe_child->pipeline = cl->pipeline;
		exit_status = execute_pipeline(exe_child, exe_child->pipeline);
		if (cl->compound_list_next)
			eval_compound_list(e, &cl->next, cl->compound_list_next);
		while (cl->next)
		{
			if (is_execute_condition(cl->condition, exit_status))
				exit_status = execute_pipeline(exe_child, cl->next->pipeline);
			cl = cl->next;
			if (cl->compound_list_next)
				eval_compound_list(e, &cl->next, cl->compound_list_next);
		}
		free(exe_child);
		exit(exit_status);
	}
	return (pid);
}
