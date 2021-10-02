#include "expansioner.h"

void		search_command_arg_node(t_ast_node *node, t_env_var *vars);
t_string	*expand_word(t_string *data, t_env_var *vars);
t_string	*expand_environment_variable(t_string *data, size_t replace_start, t_env_var *vars);

t_ast_node	*expansion(t_ast_node *node, char **envp)
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
	node->data = expand_word(node->data, vars);
}

t_string	*expand_word(t_string *data, t_env_var *vars)
{
	size_t	i;

	if (!is_expandable_string(data->start, data->len))
		return (data);
	i = 0;
	while (i < data->len)
	{
		// todo: $? expands exit status
		// todo: multiple environ like "$TEST$TEST" needs free previous data->start!
		if (data->start[i] == '$')
		{
		printf("%s\n", &data->start[i]);
			data = expand_environment_variable(data, i, vars);
		printf("%s\n", &data->start[i]);
		}
		// if (data->start[i] == '\"')
		// 	data = double_quote_string(data, i);
		if (!data)
			return (NULL);
		if (!data->start[i])
			break ;
		i++;
	}
	return (data);
}

t_string	*expand_environment_variable(t_string *data, size_t replace_start, t_env_var *vars)
{
	char			*env_value;

	env_value = search_env_vars(data, replace_start + 1, vars);
	// printf("env_key: '%s'\n", &data->start[replace_start + 1]);
	// printf("env_value: '%s'\n", env_value);
	if (!env_value)
		return (data);
	data = str_insert(data, env_value, replace_start);
	return (data);
}