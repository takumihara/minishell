#include "builtin.h"

static t_env_var	*delete_env_var(char *key, t_env_var *env_vars)
{
	t_env_var	*head_var;
	t_env_var	*pre_var;

	head_var = env_vars;
	pre_var = NULL;
	while (env_vars)
	{
		if (!ft_strcmp(key, env_vars->key))
		{
			if (pre_var)
				pre_var->next = env_vars->next;
			else
				head_var = env_vars->next;
			free(env_vars->key);
			free(env_vars->value);
			free(env_vars);
			return (head_var);
		}
		pre_var = env_vars;
		env_vars = env_vars->next;
	}
	return (head_var);
}

int	builtin_unset(int argc, char **argv, int no_use, t_env_var **env_vars)
{
	int			i;
	int			exit_status;
	char		*key;

	(void)no_use;
	exit_status = EXIT_SUCCESS;
	if (argc == 1 || !*env_vars)
		return (exit_status);
	i = 1;
	while (argv[i])
	{
		if (is_valid_argument(argv[i], ft_strlen(argv[i]), UNSET_ARG_ERROR))
		{
			key = x_strdup(argv[i]);
			*env_vars = delete_env_var(key, *env_vars);
			free(key);
		}
		else
			exit_status = EXIT_FAILURE;
		i++;
	}
	return (exit_status);
}
