#include "execute.h"

#define EXIT_STATUS_COMMAND_NOT_FOUND 127
#define EXIT_STATUS_NO_SUCH_FILE 127

int execute_command(t_executor *e, void *command, int type, bool is_last, bool is_pipe, int	pipefd[]);
int execute_simple_command(t_executor *e, t_simple_command *sc, bool is_last, bool is_pipe, int	pipefd[]);
int execute_subshell(t_executor *e, t_subshell *ss);
int execute_compound_list(t_executor *e, t_compound_list *cl);

int execute_pipeline(t_executor *e, t_pipeline *pl)
{
	int	child_pid;
	int	pipefd[2];
	int	orig_stdfd[2];
	int	child_process_cnt;
	int	statloc;
	const bool is_pipe = (pl->next != NULL);

	orig_stdfd[READ] = dup(STDIN_FILENO);
	orig_stdfd[WRITE] = dup(STDOUT_FILENO);
	pipefd[READ] = STDIN_FILENO;
	child_process_cnt = 0;
	while (pl)
	{
		dup2(pipefd[READ], STDIN_FILENO);
		if (pipefd[READ])
			close(pipefd[READ]);
		if (pl->next)
		{
			pipe(pipefd);
			dup2(pipefd[WRITE], STDOUT_FILENO);
			close(pipefd[WRITE]);
		}
		else
			dup2(orig_stdfd[WRITE], STDOUT_FILENO);
		child_pid = execute_command(e, pl->command, pl->type, !pl->next, is_pipe, pipefd);
		if (child_pid != CHILD_PROCESS_NOT_CREATED)
			child_process_cnt++;
		else if (pl->next)
			child_pid = NOT_LAST_COMMAND;
		pl = pl->next;
	}
	dup2(orig_stdfd[READ], STDIN_FILENO);
	close(orig_stdfd[READ]);
	dup2(orig_stdfd[WRITE], STDOUT_FILENO);
	close(orig_stdfd[WRITE]);
	while (child_process_cnt--)
		if (wait(&statloc) == child_pid)
			if (WIFEXITED(statloc))
				e->exit_status = WEXITSTATUS(statloc);
	return (e->exit_status);
}

int execute_command(t_executor *e, void *command, int type, bool is_last, bool is_pipe, int	pipefd[])
{
	if (type == T_SIMPLE_COMMAND)
	{
		if (((t_simple_command *)command)->err != NO_ERR)
		{
			e->exit_status = EXIT_FAILURE;
			return (CHILD_PROCESS_NOT_CREATED);
		}
		return (execute_simple_command(e, (t_simple_command *)command, is_last, is_pipe, pipefd));
	}
	else // subshell
		return (execute_subshell(e, (t_subshell *)command));
}

int execute_subshell(t_executor *e, t_subshell *ss)
{
	// redirection list process
	return (execute_compound_list(e, ss->compound_list));
}

int execute_compound_list(t_executor *e, t_compound_list *cl)
{
	pid_t			pid;
	t_executor		*exe_child;
	int				exit_status;

	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		if (!new_executor(&exe_child, NULL, NULL))
			exit(ex_perror(e, "malloc"));
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
	else if (pid < 0)
		exit(ex_perror(e, "fork"));
	return (pid);
}

// execute_simple_command returns either its child process pid or macro 'CHILD_PROCESS_NOT_CREATED'
int execute_simple_command(t_executor *e, t_simple_command *sc, bool is_last, bool is_pipe, int	pipefd[])
{
	pid_t	pid;
	char	*path;
	char	**envp;

	execute_redirect(sc);
	if (sc->argc == 0)
	{
		e->exit_status = EXIT_SUCCESS;
		return (CHILD_PROCESS_NOT_CREATED);
	}
	if (!is_pipe && execute_builtin(e, sc->argc, sc->argv, is_last))
		return (CHILD_PROCESS_NOT_CREATED);
	envp = create_envp(e);
	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		if (is_pipe && !is_last && pipefd[READ])
			close(pipefd[READ]);
		if (execute_builtin(e, sc->argc, sc->argv, is_last))
			exit(EXIT_SUCCESS);
		if (!ft_strchr(sc->argv[0], '/'))
		{
			path = get_cmd_path(e, sc->argv[0]);
			if (!path)
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(sc->argv[0], STDERR_FILENO);
				ft_putendl_fd(": command not found", STDERR_FILENO);
				exit(EXIT_STATUS_COMMAND_NOT_FOUND);
			}
		}
		if (execve(path, sc->argv, envp) == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(sc->argv[0], 2);
			ft_putendl_fd(": No such file or directory", 2);
			exit(EXIT_STATUS_NO_SUCH_FILE);
		}
	}
	else if (pid < 0)
		exit(ex_perror(e, "minishell: fork"));
	//todo: check islast
	return (pid);
}
