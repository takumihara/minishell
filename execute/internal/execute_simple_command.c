#include "execute_internal.h"
#include "exit_status.h"
#include "../../builtin/builtin.h"

static void	execute_redirect(t_simple_command *sc);
static bool	execute_builtin(t_executor *e, int ac, char **av);
static void	print_err_msg(char *path, char *msg);
static void	handle_exec_error(char *path, bool is_exec_run);

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
		if (info->is_pipe && !info->is_last)
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

void	execute_redirect(t_simple_command *sc)
{
	if (sc->r_in != UNSET_FD)
		x_dup2(sc->r_in, STDIN_FILENO);
	if (sc->r_out != UNSET_FD)
		x_dup2(sc->r_out, STDOUT_FILENO);
}

bool	execute_builtin(t_executor *e, int ac, char **av)
{
	if (!ft_strcmp(av[0], "cd"))
		e->exit_status = builtin_cd(ac, av, e->env_vars);
	else if (!ft_strcmp(av[0], "pwd"))
		e->exit_status = builtin_pwd(ac, av, e->exit_status, e->env_vars);
	else if (!ft_strcmp(av[0], "exit"))
		e->exit_status = builtin_exit(ac, av, e->exit_status);
	else if (!ft_strcmp(av[0], "echo"))
		e->exit_status = builtin_echo(ac, av);
	else if (!ft_strcmp(av[0], "export"))
		e->exit_status = builtin_export(ac, av, e->env_vars);
	else if (!ft_strcmp(av[0], "env"))
		e->exit_status = builtin_env(*e->env_vars);
	else if (!ft_strcmp(av[0], "unset"))
		e->exit_status = builtin_unset(ac, av, e->env_vars);
	else
		return (false);
	return (true);
}

void	print_err_msg(char *path, char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(msg, STDERR_FILENO);
}

void	handle_exec_error(char *path, bool is_exec_run)
{
	if (!is_exec_run)
	{
		print_err_msg(path, "command not found");
		exit(ES_COMMAND_NOT_FOUND);
	}
	else
	{
		if (access(path, F_OK) == -1)
		{
			print_err_msg(path, "No such file or directory");
			exit(ES_NO_SUCH_FILE);
		}
		else if (is_dir(path))
		{
			print_err_msg(path, "is a directory");
			exit(ES_IS_A_DIRECTORY);
		}
		else if (access(path, X_OK) == -1)
		{
			print_err_msg(path, "Permission denied");
			exit(ES_PERMISSION_DENIED);
		}
		exit(EXIT_SUCCESS);
	}
}
