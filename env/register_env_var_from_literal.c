#include "env.h"

int	register_env_var_from_literal(const char *literal_key, const char *literal_value, t_env_var **env_vars)
{
	char	*key;
	char	*value;

	key = ft_strdup(literal_key);
	if (!key)
		perror_exit("malloc", EXIT_FAILURE);
	value = ft_strdup(literal_value);
	if (!value)
		perror_exit("malloc", EXIT_FAILURE);
	return (register_env_var(key, value, env_vars));
}