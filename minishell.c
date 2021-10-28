#include "minishell.h"

int	abnormal_input_minishell(char *line, t_env_var **env_vars)
{
	int	exit_status;

	exit_status = execute(parse(lex(line)), env_vars, false);
	delete_env_lst(*env_vars);
	return (exit_status);
}

int	minishell(char *line)
{
	t_env_var	*env_vars;
	int			exit_status;

	env_vars = init_env_lst();
	register_env_var_from_literal("?", "0", 0, &env_vars);
	if (line)
		return (abnormal_input_minishell(line, &env_vars));
	exit_status = EXIT_SUCCESS;
	while (1)
	{
		set_signal_handler(READLINE_SIGNAL);
		line = readline(BLUE "minishell> " RESET);
		if (!line)
			break ;
		exit_status = execute(parse(lex(line)), &env_vars, true);
		add_history(line);
		free(line);
	}
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
