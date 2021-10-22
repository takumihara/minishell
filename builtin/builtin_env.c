#include "builtin.h"

int	builtin_env(t_env_var *env_vars)
{
	while (env_vars)
	{
		if (env_vars->value && ft_strcmp("?", env_vars->key))
			printf("%s=%s\n", env_vars->key, env_vars->value);
		env_vars = env_vars->next;
	}
	return (EXIT_SUCCESS);
}
