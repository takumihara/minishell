# include "env.h"

extern char	**environ;

t_env_var *init_env_var(char *key, char *value)
{
	t_env_var	*env_var;
    
    env_var = x_malloc(sizeof(*env_var));
	env_var->key = key;
    env_var->value = value;
    env_var->next = NULL;
    return (env_var);
}

int	delete_env_lst(t_env_var *env_vars, char *key, char *value)
{
	t_env_var	*next_env_var;

	free(key);
	free(value);
	while (env_vars)
	{
		next_env_var = env_vars->next;
		free(env_vars->key);
		free(env_vars->value);
		free(env_vars);
		env_vars = next_env_var;
	}
	return (EXIT_FAILURE);
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
		key = ft_strndup(*environ, ft_strchr(*environ, '=') - *environ);
		value = ft_strdup(ft_strchr(*environ, '=') + 1);
		if (!key || !value)
			exit(delete_env_lst(vars.next, key, value));
		tmp->next = init_env_var(key, value);
		if (!tmp->next)
			exit(delete_env_lst(vars.next, key, value));
		tmp = tmp->next;
		environ++;
	}
	return (vars.next);
}
