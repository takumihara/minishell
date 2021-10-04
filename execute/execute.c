#include "execute.h"

int execute_simple_command(t_executor *e, t_simple_command *sc);
int execute_command(t_executor *e, void *command, int type);

int execute_pipeline(t_executor *e, t_pipeline *pl)
{
	int	res;
	int	orig_stdfd[2];
	int	pipefd[2];
	int	child_process_cnt;

	orig_stdfd[READ] = dup(STDIN_FILENO);
	orig_stdfd[WRITE] = dup(STDOUT_FILENO);
	pipefd[READ] = STDIN_FILENO;
	child_process_cnt = 0;
	e->exit_status = -1;
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
		res = execute_command(e, pl->command, pl->type);
		if (res == CHILD_PROCESS_CREATED)
			child_process_cnt++;
		else if (!pl->next)
			e->exit_status = res;
		pl = pl->next;
	}
	delete_list((void *)e->pipeline, T_PIPELINE);
	dup2(orig_stdfd[READ], STDIN_FILENO);
	close(orig_stdfd[READ]);
	dup2(orig_stdfd[WRITE], STDOUT_FILENO);
	close(orig_stdfd[WRITE]);
	while (child_process_cnt--)
		wait(&res);
	if (e->exit_status == -1)
		e->exit_status = res;
	return (res);
}

int execute_command(t_executor *e, void *command, int type)
{
	if (type == T_SIMPLE_COMMAND)
		return (execute_simple_command(e, (t_simple_command *)command));
	// todo: subshell
	return (1);
}

int execute_simple_command(t_executor *e, t_simple_command *sc)
{
	pid_t	pid;
	void	*tmp;
	//todo: builtin check

	// assume there are no several redirect in
	// process redirect in
	tmp = sc->r_in;
	while (sc->r_in && sc->r_in->next)
		sc->r_in = sc->r_in->next;
	if (sc->r_in)
		dup2(sc->r_in->fd, STDIN_FILENO);
	delete_list(tmp, T_REDIRECT_IN); //todo: does the fd have to be closed here?
	// process redirect out
	sc->r_in = NULL;
	tmp = sc->r_out;
	while (sc->r_out && sc->r_out->next)
		sc->r_out = sc->r_out->next;
	if (sc->r_out) {
		dup2(sc->r_out->fd, STDOUT_FILENO);
	}
	delete_list(tmp, T_REDIRECT_OUT);
	// actual execution
	sc->r_out = NULL;
	pid = fork();
	if (pid == 0) //child process
	{
		if (execvp(sc->argv[0], sc->argv) == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(sc->argv[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			return (127);
		}
	}
	else if (pid < 0)
		exit(ex_perror(e, "fork"));
	return (CHILD_PROCESS_CREATED);
}