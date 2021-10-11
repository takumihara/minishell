#include "builtin.h"

// in expander_wildcard.c 
// static void	swap(char **a, char **b)
// {
// 	char	*tmp;

// 	tmp = *a;
// 	*a = *b;
// 	*b = tmp;
// }
// void	quick_sort(char **array, size_t left, size_t right)
// {
// 	const char	*pivot = array[left];
// 	size_t		i;
// 	size_t		j;

// 	i = left;
// 	j = right;
// 	if (left >= right)
// 		return ;
// 	while (1)
// 	{
//         while (ft_strcmp(array[i], pivot) < 0) 
//             i++;
//         while (ft_strcmp(array[j], pivot) > 0)
//             j--;
//         if (i >= j)
//             break ;
//         swap(&array[i], &array[j]);
//         i++;
//         j--;
//     }
// 	if (i)
// 		quick_sort(array, left, i - 1);
// 	if (j != right)
// 		quick_sort(array, j + 1, right);
// }

// // in expander_env.c
// char	*get_env_value(char *key, t_env_var *env_var)
// {
// 	while (env_var)
// 	{
// 		if (!ft_strcmp(key, env_var->key))
// 			return (env_var->value);
// 		env_var = env_var->next;
// 	}
// 	return (NULL);
// }


// size_t	key_strlen(const char *str)
// {
// 	size_t	len;

// 	len = 0;
// 	while (str[len] && str[len] != '=')
// 		len++;
// 	return (len);
// }

// size_t	env_vars_len(t_env_var *env_vars)
// {
// 	size_t	len;

// 	len = 0;
// 	while (env_vars)
// 	{
// 		if (ft_strcmp("_", env_vars->key))
// 			len++;
// 		env_vars = env_vars->next;
// 	}
// 	return (len);
// }

// char	**sort_env_var(char **array, t_env_var *env_vars, size_t len)
// {
// 	size_t	i;

// 	i = 0;
// 	while (env_vars)
// 	{
// 		if (ft_strcmp("_", env_vars->key))
// 		{
// 			array[i] = env_vars->key;
// 			i++;
// 		}
// 		env_vars = env_vars->next;
// 	}
// 	quick_sort(array, 0, len - 1);
// 	return (array);
// }

// static int	print_declaration(t_env_var *env_vars)
// {
// 	const size_t	len = env_vars_len(env_vars);
// 	char			**env_key_array;
// 	size_t			i;
// 	char			*value;

// 	env_key_array = malloc(sizeof(char *) * (len + 1));
// 	if (!env_key_array)
// 		return (EXIT_FAILURE);
// 	env_key_array[len] = NULL;
// 	sort_env_var(env_key_array, env_vars, len);
// 	i = 0;
// 	while (env_key_array[i])
// 	{
// 		ft_putstr_fd("declare -x ", STDOUT_FILENO);
// 		ft_putstr_fd(env_key_array[i], STDOUT_FILENO);
// 		value = get_env_value(env_key_array[i], env_vars);
// 		if (value)
// 		{
// 			ft_putstr_fd("=\"", STDOUT_FILENO);
// 			ft_putstr_fd(value, STDOUT_FILENO);
// 			ft_putstr_fd("\"\n", STDOUT_FILENO);
// 		}
// 		i++;
// 	}
// 	free(env_key_array);
// 	return (EXIT_SUCCESS);
// }

// static bool	is_valid_argument(int argc, char **argv)
// {
// 	int		i;
// 	bool	is_valid;

// 	i = 1;
// 	is_valid = true;
// 	while (i < argc)
// 	{
// 		if (argv[i][0] == '=' || argv[i][0] == '\0')
// 		{
// 			write(2, "minishell: export: `", 20);
// 			write(2, &argv[i][0], 1);
// 			write(2, "': not a valid identifier\n", 26);
// 			is_valid = false;
// 		}
// 		i++;
// 	}
// 	return (is_valid);
// }

// int	register_env_var(char *key, char *value, t_env_var *env_vars)
// {
// 	while (1)
// 	{
// 		if (!ft_strcmp(key, env_vars->key))
// 		{
// 			if (value)
// 			{
// 				free(env_vars->value);
// 				free(key);
// 				env_vars->value = value;
// 			}
// 			return (EXIT_SUCCESS);
// 		}
// 		if (!env_vars->next)
// 			break ;
// 		env_vars = env_vars->next;
// 	}
// 	env_vars->next = init_env_var(key, value);
// 	if(!env_vars)
// 	{
// 		free(key);
// 		free(value);
// 		return (EXIT_FAILURE);
// 	}
// 	return (EXIT_SUCCESS);
// }

// int	builtin_export(int argc, char **argv, int no_use, t_env_var *env_vars)
// {
// 	char	*key;
// 	char	*value;

// 	(void)no_use;
// 	value = NULL;
// 	if (argc == 1)
// 		return (print_declaration(env_vars));
// 	else if (!is_valid_argument(argc, argv))
// 		return (EXIT_FAILURE);
// 	key = ft_strndup(argv[1], key_strlen(argv[1]));
// 	if (!key)
// 		return (EXIT_FAILURE);
// 	if (ft_strchr(argv[1], '='))
// 	{
// 		value = ft_strdup(ft_strchr(argv[1], '=') + 1);
// 		if (!value)
// 		{
// 			free(key);
// 			return (EXIT_FAILURE);
// 		}
// 	}
// 	return (register_env_var(key, value, env_vars));
// }

// int main(int argc, char **argv)
// {
// 	return (builtin_export(argc, argv, 0, init_env_lst()));
// }