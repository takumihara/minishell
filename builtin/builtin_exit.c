#include "builtin.h"

int builtin_exit(int argc, char **argv, int last_exit_status, t_env_var **env_vars)
{
	int	num;

	(void)env_vars;
	ft_putendl_fd("exit", STDERR_FILENO);
	if (argc == 1)
		exit(last_exit_status);
	if (argc == 2)
	{
		if (!atoi_strict(argv[1], &num))
		{
			ft_putendl_fd("minishell: exit: ", STDERR_FILENO);
			ft_putendl_fd(argv[1], STDERR_FILENO);
			ft_putendl_fd(": numeric argument required", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		exit((unsigned char)num); //todo: free?
	}
	ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
	return (EXIT_FAILURE);
}