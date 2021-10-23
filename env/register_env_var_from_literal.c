#include "env.h"

void	register_env_var_from_literal(const char *str_key,
	const char *str_value, int num_value, t_env_var **env_vars)
{
	char	*key;
	char	*value;

	key = x_strdup(str_key);
	if (!str_value)
		value = x_itoa(num_value);
	else
		value = x_strdup(str_value);
	register_env_var(key, value, env_vars);
}
