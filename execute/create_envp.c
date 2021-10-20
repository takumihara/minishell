#include "execute.h"
#include "../wrapper/x.h"

static bool is_valid_envp_elem(t_env_var *env_var)
{
	return (ft_strcmp(env_var->key, "?") && env_var->value); //todo: idk if this is enough conditions
}

static int count_env_vars(t_env_var *env_vars)
{
	int	count;

	count = 0;
	while (env_vars)
	{
		if (is_valid_envp_elem(env_vars))
			count++;
		env_vars = env_vars->next;
	}
	return (count);
}

char	**create_envp(t_executor *e)
{
	char		**envp;
	t_env_var	*iterator;
	int			index;

	envp = x_malloc(sizeof(*envp) * (count_env_vars(*e->env_vars) + 1));
	iterator = *e->env_vars;
	index = -1;
	while (iterator)
	{
		if (is_valid_envp_elem(iterator))
			envp[++index] = strjoin_three(iterator->key, "=", iterator->value);
		iterator = iterator->next;
	}
	envp[++index] = NULL;
	return (envp);
}