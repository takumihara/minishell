#include "expander.h"
#include "internal/expander_internal.h"

static void	search_expandable_node(t_expander *e, t_ast_node *node)
{
	t_ast_node	*original_right;
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
	original_right = node->right;
	original_data = x_strdup(node->data);
	node->data = variable_expansion(e, node->data);
	if (!is_empty_data(e, node, original_data))
	{
		word_splitting(node, e, original_data, original_right);
		filename_expansion(node, e, original_data, original_right);
		quotes_removal(node, original_right);
	}
	free(original_data);
}

t_ast_node	*expand(t_ast_node *root, t_env_var **env_vars)
{
	t_expander	*e;

	if (!root)
		return (NULL);
	new_expander(&e, *env_vars);
	search_expandable_node(e, root);
	if (e->err != NO_ERR)
		return (handle_expand_error(e));
	free(e->err_data);
	free(e);
	return (root);
}
