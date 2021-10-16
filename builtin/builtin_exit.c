#include "builtin.h"

int builtin_exit(int argc, char **argv, int last_exit_status, t_env_var **env_vars)
{
	int	num;

	(void)env_vars;
	ft_putendl_fd("exit", STDERR_FILENO);
	if (argc == 1)
		exit(last_exit_status);
	if (!atoi_strict(argv[1], &num))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		exit(EXIT_STATUS_OUT_OF_RANGE);
	}
	if (argc > 2)
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	exit((unsigned char)num); //todo: free?
}