# include "env.h"

extern char	**environ;

void	print_env_vars(t_env_var *env_vars)
{
	while (env_vars)
	{
		printf("%s=%s\n", env_vars->key, env_vars->value);
		env_vars = env_vars->next;
	}
}

bool	register_key_value(char *key, char *value, t_env_var *env_vars)
{
	while (env_vars)
	{
		if (!strcmp(key, env_vars->key))
		{
			free(env_vars->value);
			env_vars->value = value;
			return (true);
		}
		env_vars = env_vars->next;
	}
	env_vars = init_env_var(key, value);
	if (!env_vars)
		return (false);
	free(key);
	key = NULL;
	return (true);
}

t_env_var *init_env_var(char *key, char *value)
{
	t_env_var	*env_var;
    
    env_var = (t_env_var *)malloc(sizeof(t_env_var));
    if (!env_var)
        return (NULL);
	env_var->key = key;
    env_var->value = value;
    env_var->next = NULL;
    return (env_var);
}

void	*delete_env_lst(t_env_var *env_vars, char *key, char *value)
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
	return (NULL);
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
		key = strndup(*environ, strchr(*environ, '=') - *environ);
		value = strdup(strchr(*environ, '=') + 1);
		if (!key || !value)
			return (delete_env_lst(vars.next, key, value));
		tmp->next = init_env_var(key, value);
		if (!tmp->next)
			return (delete_env_lst(vars.next, key, value));
		tmp = tmp->next;
		environ++;
	}
	return (vars.next);
}

int	main(void)
{
	t_env_var	*env_vars;

	env_vars = init_env_lst();
	print_env_vars(env_vars);
}