#include "builtin.h"

int		print_env_vars(t_env_var *env_vars)
{
	while (env_vars)
	{
		if (env_vars->value)
			printf("%s=%s\n", env_vars->key, env_vars->value);
		env_vars = env_vars->next;
	}
	return (EXIT_SUCCESS);
}

int		builtin_env(int argc, char **argv, int no_use, t_env_var **env_vars)
{
	(void)argc;
	(void)argv;
	(void)no_use;
	return (print_env_vars(*env_vars));
}