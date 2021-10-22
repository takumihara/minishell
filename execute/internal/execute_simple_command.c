static void	execute_redirect(t_simple_command *sc);
static bool	execute_builtin(t_executor *e, int argc, char **argv);

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

void	execute_redirect(t_simple_command *sc)
{
	if (sc->r_in != UNSET_FD)
		x_dup2(sc->r_in, STDIN_FILENO);
	if (sc->r_out != UNSET_FD)
		x_dup2(sc->r_out, STDOUT_FILENO);
}

bool	execute_builtin(t_executor *e, int argc, char **argv)
{
	if (!ft_strcmp(argv[0], "cd"))
		e->exit_status = builtin_cd(argc, argv, e->exit_status, e->env_vars);
	else if (!ft_strcmp(argv[0], "pwd"))
		e->exit_status = builtin_pwd(argc, argv, e->exit_status, e->env_vars);
	else if (!ft_strcmp(argv[0], "exit"))
		e->exit_status = builtin_exit(argc, argv, e->exit_status, e->env_vars);
	else if (!ft_strcmp(argv[0], "echo"))
		e->exit_status = builtin_echo(argc, argv, e->exit_status, e->env_vars);
	else if (!ft_strcmp(argv[0], "export"))
		e->exit_status = builtin_export(argc,
										argv, e->exit_status, e->env_vars);
	else if (!ft_strcmp(argv[0], "env"))
		e->exit_status = builtin_env(argc, argv, e->exit_status, e->env_vars);
	else if (!ft_strcmp(argv[0], "unset"))
		e->exit_status = builtin_unset(argc, argv, e->exit_status, e->env_vars);
	else
		return (false);
	return (true);
}
