#include "../execute.h"
#include "../exit_status.h"
#include "execute_internal.h"
#include "../../wrapper/x.h"

static void	get_child_process_status(t_executor *e,
				int child_process_cnt, pid_t child_pid);
static int	execute_pipeline_internal(t_executor *e,
				t_pipeline *pl, int stdout_fd, int *child_pid);
static void	save_orig_stdfd(int orig_stdfd[]);
static void	set_orig_stdfd(int orig_stdfd[]);

int	execute_pipeline(t_executor *e, t_pipeline *pl)
{
	pid_t	child_pid;
	int		orig_stdfd[2];
	int		child_process_cnt;

	save_orig_stdfd(orig_stdfd);
	child_process_cnt = execute_pipeline_internal(e,
			  pl, orig_stdfd[WRITE], &child_pid);
	set_orig_stdfd(orig_stdfd);
	get_child_process_status(e, child_process_cnt, child_pid);
	return (e->exit_status);
}

void	get_child_process_status(t_executor *e,
		 int child_process_cnt, pid_t child_pid)
{
	int		statloc;

	while (child_process_cnt--)
	{
		if (wait(&statloc) == child_pid)
		{
			if (WIFEXITED(statloc))
				e->exit_status = WEXITSTATUS(statloc);
			else if (WIFSIGNALED(statloc))
			{
				if (WTERMSIG(statloc) == SIGQUIT)
					ft_putendl_fd("Quit: 3", STDERR_FILENO);
				e->exit_status = ES_SIGNAL + WTERMSIG(statloc);
			}
		}
	}
}

int	execute_pipeline_internal(t_executor *e,
		 t_pipeline *pl, int stdout_fd, int *child_pid)
{
	t_exec_pipe_info	info;
	int					child_process_cnt;

	child_process_cnt = 0;
	info.is_pipe = (pl->next != NULL);
	info.pipefd[READ] = STDIN_FILENO;
	while (pl)
	{
		x_dup2(info.pipefd[READ], STDIN_FILENO);
		info.pipefd[READ] != STDIN_FILENO && close(info.pipefd[READ]);
		if (pl->next)
		{
			x_pipe(info.pipefd);
			x_dup2(info.pipefd[WRITE], STDOUT_FILENO);
			close(info.pipefd[WRITE]);
		}
		else
			x_dup2(stdout_fd, STDOUT_FILENO);
		info.is_last = !pl->next;
		*child_pid = execute_command(e, pl->command, pl->type, &info);
		if (*child_pid != CHILD_PROCESS_NOT_CREATED)
			child_process_cnt++;
		pl = pl->next;
	}
	return (child_process_cnt);
}

void	set_orig_stdfd(int orig_stdfd[])
{
	x_dup2(orig_stdfd[READ], STDIN_FILENO);
	close(orig_stdfd[READ]);
	x_dup2(orig_stdfd[WRITE], STDOUT_FILENO);
	close(orig_stdfd[WRITE]);
}

void	save_orig_stdfd(int orig_stdfd[])
{
	orig_stdfd[READ] = x_dup(STDIN_FILENO);
	orig_stdfd[WRITE] = x_dup(STDOUT_FILENO);
}
