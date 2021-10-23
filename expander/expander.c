#include "expander.h"
#include "internal/expander_internal.h"

static char	*expand_word(t_expander *e, char *data, char delimiter)
{
	int		status;
	size_t	i;

	i = 0;
	status = OUTSIDE;
	while (data[i])
	{
		status = quotation_status(data[i], status);
		if (data[i] == '$' && delimiter == '$' && status != IN_SINGLE_QUOTE)
			data = expand_environment_variable(data, i, e, status);
		else if (data[i] == '*' && delimiter == '*' && status == OUTSIDE)
			data = expand_wildcard(data, i);
		if (!data[i])
			break ;
		i++;
	}
	return (data);
}

static void	search_expandable_node(t_expander *e, t_ast_node *node)
{
	char		*original_data;

	if (!node)
		return ;
	search_expandable_node(e, node->right);
	search_expandable_node(e, node->left);
	if (node->type != COMMAND_ARG_NODE
		&& node->type != REDIRECT_IN_NODE
		&& node->type != REDIRECT_OUT_NODE
		&& node->type != REDIRECT_APPEND_NODE)
		return ;
	original_data = x_strdup(node->data);
	node->data = expand_word(e, node->data, '$');
	if (!*node->data && node->type == COMMAND_ARG_NODE)
	{
		node->data = NULL;
		free(original_data);
		return ;
	}
	node->data = expand_word(e, node->data, '*');
	// if (!is_valid_expansion(e, ))
	word_splitting(node, e, original_data);
	free(original_data);
}

t_ast_node	*expand(t_ast_node *root, t_env_var **env_vars)
{
	t_expander	*e;

	if (!root)
		return (NULL);
	new_expander(&e, *env_vars);
	search_command_arg_node(e, root);
	if (e->err != NO_ERR)
		return (handle_expand_error(e));
	free(e->err_data);
	free(e);
	return (root);
}
