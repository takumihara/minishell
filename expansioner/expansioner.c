#include "expansioner.h"

void		search_command_arg_node(t_ast_node *node);
t_string	*expand_word(t_string *data);
t_string	*expand_environment_variable(t_string *data, size_t replace_start);

t_ast_node	*expansion(t_ast_node *node, char **envp)
{
	t_ast_node	*root;
	// todo: use envp to expand 'export' command
	(void)envp;
	root = node;
	search_command_arg_node(node);
	
	return (root);
}

void	search_command_arg_node(t_ast_node *node)
{
	if (!node)
		return ;
	search_command_arg_node(node->right);
	search_command_arg_node(node->left);
	if (node->type != COMMAND_ARG_NODE)
		return ;
	node->data = expand_word(node->data);
}

t_string	*expand_word(t_string *data)
{
	size_t	i;

	if (!is_expandable_string(data->start))
		return (data);
	i = 0;
	while (i < data->len)
	{
		if (data->start[i] == '$')
			data = expand_environment_variable(data, i);
		if (!data)
			return (NULL);
		i++;
	}
	return (data);
}

t_string	*expand_environment_variable(t_string *data, size_t replace_start)
{
	char			*env_var;
	char			*env_value;

	env_var = cut_environment_variable(&data->start[replace_start + 1]);
	if (!env_var)
		return (NULL);
	env_value = getenv(env_var);
	if (!env_value)
		return (data);
	data = str_insert(data, env_var, env_value, replace_start);
	free(env_var);
	return (data);
}