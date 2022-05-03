#include "expander_internal.h"

static char	**row_malloc_split(char const *str, const char *delims, size_t *row)
{
	size_t	len;
	char	**split;

	len = 0;
	while (*str)
	{
		if (!is_delims(*str, delims))
		{
			while (!is_delims(*str, delims) && *str)
			{
				if (is_quote(*str))
					str += skip_quotes(str, *str);
				str++;
			}
			len++;
		}
		else
			str++;
	}
	split = x_malloc(sizeof(*split) * (len + 1));
	split[len] = NULL;
	*row = len;
	return (split);
}

static char	*ft_strdup_split(char const *src, const char *delims)
{
	size_t	i;
	size_t	len;
	char	*str;

	len = 0;
	while (!is_delims(src[len], delims) && src[len])
	{
		if (is_quote(src[len]))
			len += skip_quotes(&src[len], src[len]);
		len++;
	}
	str = x_malloc(sizeof(*str) * (len + 1));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		str[i] = src[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

static char	**split_by_space_skip_quotes(char const *str, const char *delims)
{
	size_t	i;
	size_t	j;
	size_t	row;
	char	**split;

	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	split = row_malloc_split(str, delims, &row);
	while (i < row)
	{
		while (is_delims(str[j], delims))
			j++;
		split[i++] = ft_strdup_split(&str[j], delims);
		while (!is_delims(str[j], delims) && str[j])
		{
			if (is_quote(str[j]))
				j += skip_quotes(&str[j], str[j]);
			j++;
		}
	}
	return (split);
}

static bool	split_arg_node(char **split, t_ast_node *node,
		char *expanded, const t_ast_node *original_right)
{
	int					i;
	t_ast_node			*result;

	i = -1;
	while (split[++i])
	{
		if (i == 0)
		{
			if (node->type != COMMAND_ARG_NODE && node->type != HEREDOC_NODE
				&& ft_strcmp(split[i], expanded))
				return (false);
			free(node->data);
			node->data = split[i];
		}
		else
		{
			new_ast_node(&result);
			result->data = split[i];
			result->type = COMMAND_ARG_NODE;
			node->right = result;
			node = node->right;
		}
	}
	node->right = (t_ast_node *)original_right;
	return (true);
}

void	word_splitting(t_ast_node *node, t_expander *e, char *original_data,
	const t_ast_node *original_right)
{
	char	**split;
	char	*expanded_data;

	expanded_data = x_strdup(node->data);
	remove_null_argument(node->data);
	split = split_by_space_skip_quotes(node->data, " \t\n");
	if (!split_arg_node(split, node, expanded_data, original_right))
	{
		free_2d_array((void ***)&split);
		expand_redirect_error(original_data, e);
	}
	free(expanded_data);
	free(split);
}
