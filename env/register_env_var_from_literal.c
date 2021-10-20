#include "env.h"

int	register_env_var_from_literal(const char *str_key, const char *str_value, int num_value, t_env_var **env_vars)
{
	char	*key;
	char	*value;

	key = ft_strdup(str_key);
	if (!key)
		perror_exit("malloc", EXIT_FAILURE);
	if (!str_value)
		value = ft_itoa(num_value);
	else
		value = ft_strdup(str_value);
	if (!value)
		perror_exit("malloc", EXIT_FAILURE);
	return (register_env_var(key, value, env_vars));
}