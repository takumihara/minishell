// #include "expander.h"

// t_env_var	*split_environment_vars(char **envp)
// {
// 	t_env_var	vars;
// 	t_env_var	*tmp;

// 	vars.next = NULL;
// 	tmp = &vars;
// 	while (*envp)
// 	{
// 		tmp->next = (t_env_var *)malloc(sizeof(t_env_var));
// 		if (!tmp->next)
// 		{
// 			env_lstclear(vars.next);
// 			return (NULL);
// 		}
// 		tmp->next->next = NULL;
// 		set_key_value(*envp, tmp->next);
// 		tmp = tmp->next;
// 		envp++;
// 	}
// 	return (vars.next);
// }

// void		set_key_value(char *envp, t_env_var *vars)
// {
// 	size_t	len;

// 	len = 0;
// 	vars->key = envp;
// 	while (envp[len])
// 	{
// 		if (envp[len] == '=')
// 		{
// 			vars->value = &envp[len + 1];
// 			return ;
// 		}
// 		len++;
// 	}
// }

// void	env_lstclear(t_env_var *lst)
// {
// 	t_env_var	*tmp;

// 	if (!lst)
// 		return ;
// 	while (lst)
// 	{
// 		tmp = lst->next;
// 		free(lst);
// 		lst = tmp;
// 	}
// }

// // todo: remove test function.
// void	print_env_lst(t_env_var *vars)
// {
// 	size_t	i = 0;

// 	while (vars)
// 	{
// 		printf("index :[%zu]\n", i);
// 		printf("key :%s\n", vars->key);
// 		printf("value :%s\n\n", vars->value);
// 		i++;
// 		vars = vars->next;
// 	}
// }
