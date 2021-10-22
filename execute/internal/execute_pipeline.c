#include "../execute.h"
#include "exit_status.h"
#include "execute_internal.h"
#include "../../wrapper/x.h"

int	execute_command(t_executor *e,
		   void *command, int type, t_exec_pipe_info *info);
int	execute_subshell(t_executor *e, t_subshell *ss);
int	execute_compound_list(t_executor *e, t_compound_list *cl);
int	execute_simple_command(t_executor *e,
		  t_simple_command *sc, t_exec_pipe_info *info);

void	get_child_process_status(t_executor *e, int child_process_cnt, pid_t child_pid)
{
	int		statloc;

	while (child_process_cnt--)
	{
		if (wait(&statloc) == child_pid)
		{
			if (WIFEXITED(statloc))
				e->exit_status = WEXITSTATUS(statloc);
			else if (WIFSIGNALED(statloc))
				e->exit_status = ES_SIGNAL + WTERMSIG(statloc);
//			else if (WIFSTOPPED(statloc))
//				e->exit_status = ES_SIGNAL + WSTOPSIG(statloc);
		}
	}
}

//void	execute_pipeline_internal(t_executor *e, t_pipeline *pl)
//{
//	if (!pl)
//		return ;
//	x_dup2(info.pipefd[READ], STDIN_FILENO);
//	if (info.pipefd[READ])
//		close(info.pipefd[READ]);
//	if (pl->next)
//	{
//		x_pipe(info.pipefd);
//		x_dup2(info.pipefd[WRITE], STDOUT_FILENO);
//		close(info.pipefd[WRITE]);
//	}
//	else
//		x_dup2(orig_stdfd[WRITE], STDOUT_FILENO);
//	info.is_last = !pl->next;
//	child_pid = execute_command(e, pl->command, pl->type, &info);
//	if (child_pid != CHILD_PROCESS_NOT_CREATED)
//		child_process_cnt++;
//	execute_pipeline_internal(e, pl->next);
//}

int	execute_pipeline(t_executor *e, t_pipeline *pl)
{
	pid_t 		child_pid;
	int			orig_stdfd[2];
	int			child_process_cnt;
	t_exec_pipe_info	info;

	info.is_pipe = (pl->next != NULL);
	orig_stdfd[READ] = x_dup(STDIN_FILENO);
	orig_stdfd[WRITE] = x_dup(STDOUT_FILENO);
	info.pipefd[READ] = STDIN_FILENO;
	child_process_cnt = 0;
	while (pl)
	{
		x_dup2(info.pipefd[READ], STDIN_FILENO);
		if (info.pipefd[READ])
			close(info.pipefd[READ]);
		if (pl->next)
		{
			x_pipe(info.pipefd);
			x_dup2(info.pipefd[WRITE], STDOUT_FILENO);
			close(info.pipefd[WRITE]);
		}
		else
			x_dup2(orig_stdfd[WRITE], STDOUT_FILENO);
		info.is_last = !pl->next;
		child_pid = execute_command(e, pl->command, pl->type, &info);
		if (child_pid != CHILD_PROCESS_NOT_CREATED)
			child_process_cnt++;
		pl = pl->next;
	}
	x_dup2(orig_stdfd[READ], STDIN_FILENO);
	close(orig_stdfd[READ]);
	x_dup2(orig_stdfd[WRITE], STDOUT_FILENO);
	close(orig_stdfd[WRITE]);
	get_child_process_status(e, child_process_cnt, child_pid);
	return (e->exit_status);
}

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
		new_executor(&exe_child, NULL, NULL);
		exe_child->pipeline = cl->pipeline;
		exit_status = execute_pipeline(exe_child, exe_child->pipeline);
		if (cl->compound_list_next)
			 init_compound_list(e, &cl->next, cl->compound_list_next);
		while (cl->next)
		{
			if (is_execute_condition(cl->condition, exit_status))
				exit_status = execute_pipeline(exe_child, cl->next->pipeline);
			cl = cl->next;
			if (cl->compound_list_next)
				init_compound_list(e, &cl->next, cl->compound_list_next);
		}
		free(exe_child);
		exit(exit_status);
	}
	return (pid);
}

// execute_simple_command returns either child process pid
// or macro 'CHILD_PROCESS_NOT_CREATED'
int	execute_simple_command(t_executor *e,
			  t_simple_command *sc, t_exec_pipe_info *info)
{
	pid_t	pid;
	char	*path;

	execute_redirect(sc);
	if (sc->argc == 0)
	{
		e->exit_status = EXIT_SUCCESS;
		return (CHILD_PROCESS_NOT_CREATED);
	}
	if (!info->is_pipe && execute_builtin(e, sc->argc, sc->argv))
		return (CHILD_PROCESS_NOT_CREATED);
	pid = x_fork();
	if (pid == CHILD_PROCESS)
	{
		if (info->is_pipe && !info->is_last && info->pipefd[READ])
			close(info->pipefd[READ]);
		if (execute_builtin(e, sc->argc, sc->argv))
			exit(EXIT_SUCCESS);
		path = get_cmd_path(e, sc->argv[0]);
		if (!path)
			handle_exec_error(sc->argv[0], false);
		if (execve(path, sc->argv, create_envp(e)) == -1)
			handle_exec_error(path, true);
	}
	return (pid);
}
