#include "env.h"

extern char	**environ;

t_env_var	*init_env_var(char *key, char *value)
{
	t_env_var	*env_var;

	env_var = x_malloc(sizeof(*env_var));
	env_var->key = key;
	env_var->value = value;
	env_var->next = NULL;
	return (env_var);
}

void	delete_env_lst(t_env_var *env_vars)
{
	t_env_var	*next_env_var;

	while (env_vars)
	{
		next_env_var = env_vars->next;
		free(env_vars->key);
		free(env_vars->value);
		free(env_vars);
		env_vars = next_env_var;
	}
}

t_env_var	*init_env_lst(void)
{
	t_env_var	vars;
	t_env_var	*tmp;
	char		*key;
	char		*value;

	vars.next = NULL;
	tmp = &vars;
	while (*environ)
	{
		key = x_strndup(*environ, ft_strchr(*environ, '=') - *environ);
		value = x_strdup(ft_strchr(*environ, '=') + 1);
		tmp->next = init_env_var(key, value);
		tmp = tmp->next;
		environ++;
	}
	return (vars.next);
}
