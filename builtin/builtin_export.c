#include "builtin.h"

static size_t	key_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (str[len] && str[len] != '=')
		len++;
	return (len);
}

static size_t	env_vars_len(t_env_var *env_vars)
{
	size_t	len;

	len = 0;
	while (env_vars)
	{
		if (ft_strcmp("_", env_vars->key) && ft_strcmp("?", env_vars->key))
			len++;
		env_vars = env_vars->next;
	}
	return (len);
}

static char	**sort_env_var(char **array, t_env_var *env_vars, size_t len)
{
	size_t	i;

	i = 0;
	while (env_vars)
	{
		if (ft_strcmp("_", env_vars->key) && ft_strcmp("?", env_vars->key))
		{
			array[i] = env_vars->key;
			i++;
		}
		env_vars = env_vars->next;
	}
	quick_sort(array, 0, len - 1);
	return (array);
}

static int	print_declaration(t_env_var *env_vars)
{
	const size_t	len = env_vars_len(env_vars);
	char			**env_key_array;
	size_t			i;
	char			*value;

	if (!env_vars)
		return (EXIT_SUCCESS);
	env_key_array = x_malloc(sizeof(char *) * (len + 1));
	env_key_array[len] = NULL;
	sort_env_var(env_key_array, env_vars, len);
	i = 0;
	while (env_key_array[i])
	{
		printf("declare -x %s", env_key_array[i]);
		value = get_env_value(env_key_array[i], env_vars);
		if (value)
			printf("=\"%s\"", value);
		printf("\n");
		i++;
	}
	free(env_key_array);
	return (EXIT_SUCCESS);
}

int	builtin_export(int argc, char **argv, t_env_var **env_vars)
{
	char	*key;
	char	*value;
	int		i;
	int		exit_status;

	exit_status = EXIT_SUCCESS;
	if (argc == 1)
		return (print_declaration(*env_vars));
	i = 1;
	while (i < argc)
	{
		value = NULL;
		if (is_valid_argument(argv[i], key_strlen(argv[i]), EXPORT_ARG_ERROR))
		{
			key = x_strndup(argv[i], key_strlen(argv[i]));
			if (ft_strchr(argv[i], '='))
				value = x_strdup(ft_strchr(argv[i], '=') + 1);
			register_env_var(key, value, env_vars);
		}
		else
			exit_status = EXIT_FAILURE;
		i++;
	}
	return (exit_status);
}
