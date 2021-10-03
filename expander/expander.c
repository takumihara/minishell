#include "expander.h"

void	search_command_arg_node(t_ast_node *node, t_env_var *vars);
char	*expand_word(char *data, t_env_var *vars);
char	*expand_environment_variable(char *data, size_t replace_start, t_env_var *vars);

t_ast_node	*expand(t_ast_node *node, char **envp)
{
	t_ast_node	*root;
	t_env_var	*vars;

	vars = split_environment_vars(envp);
	if (!vars)
		return (NULL);
	// todo: remove this. To confirm env list.
	// print_env_lst(vars);
	root = node;
	search_command_arg_node(node, vars);
	env_lstclear(vars);
	return (root);
}

void	search_command_arg_node(t_ast_node *node, t_env_var *vars)
{
	if (!node)
		return ;
	search_command_arg_node(node->right, vars);
	search_command_arg_node(node->left, vars);
	if (node->type != COMMAND_ARG_NODE)
		return ;
	// todo: export env_var
	// if (!ft_strcmp(node->data, "export"))
	// 	export_env_var();
	node->data = expand_word(node->data, vars);
}

char	*expand_word(char *data, t_env_var *vars)
{
	size_t	i;

	if (!is_expandable_string(data))
		return (data);
	i = 0;
	while (data[i])
	{
		// todo: $? expands exit status
		// todo: multiple environ like "$TEST$TEST" needs free previous data->start!
		if (data[i] == '$')
		{
			printf("%s\n", data);
			data = expand_environment_variable(data, i, vars);
			printf("%s\n", data);
		}
		// if (data->start[i] == '\"')
		// 	data = double_quote_string(data, i);
		if (!data)
			return (NULL);
		if (!data[i])
			break ;
		i++;
	}
	return (data);
}

char	*expand_environment_variable(char *data, size_t replace_start, t_env_var *vars)
{
	char			*env_value;

	env_value = search_env_vars(data, replace_start + 1, vars);
	// printf("env_key: '%s'\n", &data->start[replace_start + 1]);
	// printf("env_value: '%s'\n", env_value);
	if (!env_value)
		return (data);
	data = str_insert(data, replace_start, env_value, ft_strlen(env_value));
	return (data);
}