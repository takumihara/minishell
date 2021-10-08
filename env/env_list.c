# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <stdbool.h>

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
			env_vars->key = value;
			return (true);
		}
		env_vars = env_vars->next;
	}
	env_vars = init_node(key, value);
	if (!env_vars)
		return (false);
	free(key);
	key = NULL;
	return (true);
}

t_env_var *init_node(char *key, char *value)
{
	t_env_var	*env_node;
    
    env_node = (t_env_var *)malloc(sizeof(t_env_var));
    if (!env_node)
        return (NULL);
	env_node->key = key;
    env_node->value = value;
    env_node->next = NULL;
    return (env_node);
}

void	*delete_env_lst(t_env_var *env_vars, char *key, char *value)
{
	(void)env_vars;
	if (key)
		free(key);
	if (value)
		free(value);
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
	key = NULL;
	value = NULL;
	while (*environ)
	{
		key = strndup(*environ, strchr(*environ, '=') - *environ);
		value = strdup(strchr(*environ, '=') + 1);
		if (!key || !value)
			return (delete_env_lst(vars.next, key, value));
		tmp->next = init_node(key, value);
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