#include "expander_internal.h"

static bool
	expand_wildcard(t_ast_node *node, size_t pre_len, t_ast_node *tmp_right)
{
	char	*original_data;
	char	*removed_stars_data;

	original_data = x_strdup(node->data);
	node->data = remove_multi_stars(node->data);
	removed_stars_data = x_strdup(node->data);
	expand_matching_pattern(node, node->data, pre_len, tmp_right);
	if (!ft_strcmp(removed_stars_data, node->data) && node->right == tmp_right)
	{
		free(removed_stars_data);
		free(node->data);
		node->data = original_data;
	}
	else
	{
		free(removed_stars_data);
		free(original_data);
		if (node->type != COMMAND_ARG_NODE)
			return (false);
	}
	return (true);
}

void	filename_expansion(t_ast_node *node, t_expander *e, char *original_data,
	const t_ast_node *original_right)
{
	t_ast_node	*tmp_right;
	size_t		pre_len;

	while (node != original_right)
	{
		tmp_right = node->right;
		if (contain_expandable_star(node->data, &pre_len))
			if (!expand_wildcard(node, pre_len, tmp_right))
				expand_redirect_error(original_data, e);
		node = tmp_right;
	}
}
