#include "builtin.h"

int	builtin_pwd(int argc, char **argv, int no_use, t_env_var *env_vars)
{
	char	*cwd;

	(void)argc;
	(void)argv;
	(void)no_use;
	(void)env_vars;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("minishell: pwd"); //todo: cannot reproduce EACCES
		return (EXIT_FAILURE);
	}
	ft_putendl_fd(cwd, 1);
	free(cwd);
	return (EXIT_SUCCESS);
}