#include "minishell.h"

int	minishell_command(char *line, t_env_var **env_vars)
{
	int	exit_status;

	exit_status = execute(parse(lex(line)), env_vars, false);
	delete_env_lst(*env_vars);
	return (exit_status);
}

int	minishell_repl(t_env_var **env_vars)
{
	char	*line;
	int		exit_status;

	exit_status = EXIT_SUCCESS;
	while (1)
	{
		g_signal = -1;
		set_signal_handler(READLINE_SIGNAL);
		line = readline(BLUE "minishell> " RESET);
		if (!line)
			break ;
		if (!ft_strcmp(line, ""))
		{
			free(line);
			continue ;
		}
		if (g_signal == SIGINT)
			register_env_var_from_literal("?", NULL, 1, env_vars);
		add_history(line);
		exit_status = execute(parse(lex(line)), env_vars, true);
		free(line);
	}
	return (exit_status);
}

int	minishell(char *line)
{
	t_env_var	*env_vars;
	int			exit_status;

	env_vars = init_env_lst();
	register_env_var_from_literal("?", NULL, 0, &env_vars);
	if (line)
		return (minishell_command(line, &env_vars));
	exit_status = minishell_repl(&env_vars);
	delete_env_lst(env_vars);
	ft_putstr_fd("exit\n", STDERR_FILENO);
	return (exit_status);
}

int	main(int argc, char **argv)
{
	char			*piped_lines;
	int				exit_status;

	if (argc == 3 && !ft_strcmp(argv[1], "-c"))
		return (minishell(argv[2]));
	else
	{
		if (isatty(STDIN_FILENO) == 0)
		{
			piped_lines = read_all(STDIN_FILENO);
			if (piped_lines[0])
			{
				exit_status = minishell(piped_lines);
				free(piped_lines);
				return (exit_status);
			}
			free(piped_lines);
		}
		return (minishell(NULL));
	}
}
