#include "env.h"

char	*get_env_value(char *key, t_env_var *env_var)
{
	while (env_var)
	{
		if (!ft_strcmp(key, env_var->key))
			return (env_var->value);
		env_var = env_var->next;
	}
	return (NULL);
}
