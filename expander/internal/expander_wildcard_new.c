#include "expander_internal.h"

static bool	expand_wildcard(t_ast_node *node, size_t pre_len)
{
	char	*original_data;
	char	*removed_stars_data;

	original_data = x_strdup(node->data);
	node->data = remove_multi_stars(node->data);
	removed_stars_data = x_strdup(node->data);
	
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
		{
			if (!expand_wildcard(node, pre_len))
			{
				expand_redirect_error(original_data, e);
				return ;
			}
		}
		node = tmp_right;
	}
}
