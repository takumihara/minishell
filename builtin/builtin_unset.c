#include "builtin.h"

bool	print_argument_error(char *argv)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(argv, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	return (false);
}

static bool	is_valid_argument(char *argv)
{
	const size_t	len = strlen(argv);
	size_t			i;

	if (!len)
		return (print_argument_error(argv));
	i = 0;
	while (i < len)
	{
		if (i == 0 && !ft_isalpha(argv[i]))
			return (print_argument_error(argv));
		else if (i != 0 && !ft_isalnum(argv[i]))
			return (print_argument_error(argv));
		i++;
	}
	return (true);
}

bool	delete_env_var(char *key, t_env_var *env_vars)
{
	t_env_var	*pre_var;

	pre_var = NULL;
	while (env_vars)
	{
		if (!ft_strcmp(key, env_vars->key))
		{
			if (pre_var)
				pre_var->next = env_vars->next;
			free(env_vars->key);
			free(env_vars->value);
			free(env_vars);
			if (!pre_var)
				return (false);
			else
				return (true);
		}
		pre_var = env_vars;
		env_vars = env_vars->next;
	}
	return (true);
}

int		builtin_unset(int argc, char **argv, int no_use, t_env_var **env_vars)
{
	int			i;
	int			exit_status;
	char		*key;
	t_env_var	*next_var;

	(void)no_use;
	exit_status = EXIT_SUCCESS;
	if (argc == 1)
		return (exit_status);
	if (*env_vars)
		next_var = (*env_vars)->next;
	i = 1;
	while (argv[i])
	{
		if (is_valid_argument(argv[i]))
		{
			key = ft_strdup(argv[i]);
			if (!key)
				return (BUILTIN_MALLOC_ERROR);
			if (!delete_env_var(key, *env_vars))
				*env_vars = next_var;
			free(key);
		}
		else
			exit_status = EXIT_FAILURE;
		i++;
	}
	return (exit_status);
}
