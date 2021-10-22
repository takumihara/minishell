#include "builtin.h"

int	builtin_cd(int argc, char **argv, int no_use, t_env_var **env_vars)
{
	char	*path;

	(void)no_use;
	if (argc == 1)
	{
		path = get_env_value("HOME", *env_vars);
		if (!path)
		{
			ft_putendl_fd("minishell: cd: HOME not set", 2);
			return (EXIT_FAILURE);
		}
		else if (!path[0])
			return (EXIT_SUCCESS);
	}
	else if (argv[1][0] == '\0')
		return (EXIT_SUCCESS);
	else
		path = argv[1];
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		perror(path);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}