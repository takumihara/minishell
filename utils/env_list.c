#include "expander.h"

extern char	**environ;

void		set_key_value(char *envp, t_env_var *vars)
{
	size_t	len;

	len = 0;
	vars->key = envp;
	while (envp[len])
	{
		if (envp[len] == '=')
		{
			vars->value = &envp[len + 1];
			return ;
		}
		len++;
	}
}

void	env_lstclear(t_env_var *lst)
{
	t_env_var	*tmp;

	if (!lst)
		return ;
	while (lst)
	{
		tmp = lst->next;
		free(lst);
		lst = tmp;
	}
}


void	print_env_vars(t_env_var *env_vars)
{
	while (env_vars)
	{
		printf("%s=%s\n", env_vars->key, env_vars->value);
		env_vars = env_vars->next;
	}
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

bool	register_key_value(char *key, char *value, t_env_var *env_vars)
{
	while (env_vars)
	{
		if (!ft_strcmp(key, env_vars->key))
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
		tmp->next = (t_env_var *)malloc(sizeof(t_env_var));
		if (!tmp->next)
			return (delete_env_lst(vars.next, key, value));
		if (!register_key_value(key, value, vars.next))
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