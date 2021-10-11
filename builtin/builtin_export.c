#include "builtin.h"

size_t	key_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (str[len] && str[len] != '=')
		len++;
	return (len);
}

size_t	env_vars_len(t_env_var *env_vars)
{
	size_t	len;

	len = 0;
	while (env_vars)
	{
		if (ft_strcmp("_", env_vars->key))
			len++;
		env_vars = env_vars->next;
	}
	return (len);
}

char	**sort_env_var(char **array, t_env_var *env_vars, size_t len)
{
	size_t	i;

	i = 0;
	while (env_vars)
	{
		if (ft_strcmp("_", env_vars->key))
		{
			array[i] = env_vars->key;
			i++;
		}
		env_vars = env_vars->next;
	}
	quick_sort(array, 0, len - 1);
	return (array);
}

int	print_declaration(t_env_var *env_vars)
{
	const size_t	len = env_vars_len(env_vars);
	char			**env_key_array;
	size_t			i;
	char			*value;

	env_key_array = malloc(sizeof(char *) * (len + 1));
	if (!env_key_array)
		return (BUILTIN_MALLOC_ERROR);
	env_key_array[len] = NULL;
	sort_env_var(env_key_array, env_vars, len);
	i = 0;
	while (env_key_array[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(env_key_array[i], STDOUT_FILENO);
		value = get_env_value(env_key_array[i], env_vars);
		if (value)
		{
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(value, STDOUT_FILENO);
			ft_putstr_fd("\"", STDOUT_FILENO);
		}
		ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}
	free(env_key_array);
	return (EXIT_SUCCESS);
}

bool	is_valid_argument(int argc, char **argv)
{
	int		i;
	bool	is_valid;

	i = 1;
	is_valid = true;
	while (i < argc)
	{
		if (argv[i][0] == '=' || argv[i][0] == '\0')
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putchar_fd(argv[i][0], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			is_valid = false;
		}
		i++;
	}
	return (is_valid);
}

t_env_var	*search_env_key(char *key, t_env_var *env_vars, bool *exist)
{
	while (1)
	{
		if (!ft_strcmp(key, env_vars->key))
		{
			*exist = true;
			return (env_vars);
		}
		if (!env_vars->next)
			break ;
		env_vars = env_vars->next;
	}
	*exist = false;
	return (env_vars);
}

int	register_env_var(char *key, char *value, t_env_var *env_vars)
{
	t_env_var	*target_var;
	bool		exist;

	exist = false;
	target_var = search_env_key(key, env_vars, &exist);
	if (exist && value)
	{
		free(key);
		free(target_var->value);
		target_var->value = value;
	}
	else if (!exist)
	{
		target_var->next = init_env_var(key, value);
		if (!target_var->next)
		{
			free(key);
			free(value);
			return (BUILTIN_MALLOC_ERROR);
		}
	}
	return (EXIT_SUCCESS);
}

int	builtin_export(int argc, char **argv, int no_use, t_env_var *env_vars)
{
	char	*key;
	char	*value;

	(void)no_use;
	value = NULL;
	if (argc == 1)
		return (print_declaration(env_vars));
	else if (!is_valid_argument(argc, argv))
		return (EXIT_FAILURE);
	key = ft_strndup(argv[1], key_strlen(argv[1]));
	if (!key)
		return (BUILTIN_MALLOC_ERROR);
	if (ft_strchr(argv[1], '='))
	{
		value = ft_strdup(ft_strchr(argv[1], '=') + 1);
		if (!value)
		{
			free(key);
			return (BUILTIN_MALLOC_ERROR);
		}
	}
	return (register_env_var(key, value, env_vars));
}
