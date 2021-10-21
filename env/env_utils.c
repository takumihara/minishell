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

static t_env_var	*search_env_key(char *key, t_env_var *env_vars, bool *exist)
{
	while (env_vars)
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

void	register_env_var(char *key, char *value, t_env_var **env_vars)
{
	t_env_var	*target_var;
	bool		exist;

	target_var = search_env_key(key, *env_vars, &exist);
	if (exist)
	{
		free(key);
		if (value)
		{
			free(target_var->value);
			target_var->value = value;
		}
	}
	else
	{
		if (target_var)
			target_var->next = init_env_var(key, value);
		else if (!*env_vars)
			*env_vars = init_env_var(key, value);
	}
}
