#include "execute.h"

int execute_command(t_executor *e, void *command, int type, bool islast, int orig_stdfd[]);
int execute_simple_command(t_executor *e, t_simple_command *sc, bool islast, int orig_stdfd[]);
int execute_subshell(t_executor *e, t_subshell *ss);
int execute_compound_list(t_executor *e, t_compound_list *cl);

int execute_pipeline(t_executor *e, t_pipeline *pl)
{
	int	child_pid;
	int	orig_stdfd[2];
	int	pipefd[2];
	int	child_process_cnt;
	int	statloc;

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
		child_pid = execute_command(e, pl->command, pl->type, !pl->next, orig_stdfd);
		if (child_pid != CHILD_PROCESS_NOT_CREATED)
			child_process_cnt++;
		else if (pl->next)
			child_pid = NOT_LAST_COMMAND;
		pl = pl->next;
	}
	delete_list((void *)e->pipeline, T_PIPELINE);
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

int execute_command(t_executor *e, void *command, int type, bool islast, int orig_stdfd[])
{
	if (type == T_SIMPLE_COMMAND)
		return (execute_simple_command(e, (t_simple_command *)command, islast, orig_stdfd));
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
	pid_t	pid;
	t_compound_list *cl_next;
	t_executor *exe_child;

	pid = fork();
	cl_next = NULL;
	if (pid == CHILD_PROCESS)
	{
		if (!new_executor(&exe_child, NULL))
			exit(ex_perror(e, "malloc"));
		cl->exit_status = execute_pipeline(exe_child, cl->pipeline);
		if (cl->compound_list_next)
			cl_next = init_compound_list(e, cl->compound_list_next);
		while (cl_next)
		{
			if (cl->condition == CONDITION_NL || cl->condition == cl->exit_status)
				cl_next->exit_status = execute_pipeline(exe_child, cl_next->pipeline);
			// free(cl);
			// free(exe_child);
			cl = cl_next;
			if (cl->compound_list_next)
				cl_next = init_compound_list(e, cl->compound_list_next);
			else
				exit(0);
		}
		exit(0); // idk if this matters
	}
	else if (pid < 0)
		exit(ex_perror(e, "fork"));
	wait(NULL);
	return (pid);
}

// execute_simple_command returns either its child process pid or macro 'CHILD_PROCESS_NOT_CREATED'
int execute_simple_command(t_executor *e, t_simple_command *sc, bool islast, int orig_stdfd[])
{
	pid_t	pid;

	execute_redirect(e, sc, orig_stdfd);
	// todo: execute_heredoc
	if (execute_builtin(e, sc->argc, sc->argv, islast))
		return (CHILD_PROCESS_NOT_CREATED);
	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		if (execvp(sc->argv[0], sc->argv) == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(sc->argv[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			exit(INEXECUTABLE);
		}
	}
	else if (pid < 0)
		exit(ex_perror(e, "minishell: fork"));
	//todo: check islast
	return (pid);
}