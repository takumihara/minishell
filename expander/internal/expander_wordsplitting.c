#include "expander_internal.h"

static bool	is_delims(char c, char const *delims)
{
	size_t	j;

	j = 0;
	while (delims[j])
	{
		if (delims[j] == c)
			return (true);
		j++;
	}
	return (false);
}

static size_t	skip_quotes(const char *str, char quote_type)
{
	size_t	len;

	len = 1;
	while (str[len] && str[len] != quote_type)
		len++;
	return (len);
}

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

char	**split_by_space_skip_quotes(char const *str, const char *delims)
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

static bool	split_arg_node(char **split, t_ast_node *node, char *expanded_data)
{
	const t_ast_node	*original_right = node->right;
	int					i;
	t_ast_node			*result;

	i = -1;
	while (split[++i])
	{
		if (i == 0)
		{
			if (node->type != COMMAND_ARG_NODE && ft_strcmp(split[i], expanded_data))
				return (false);
			free(node->data);
			node->data = remove_quotes(split[i]);
		}
		else
		{
			new_ast_node(&result);
			result->data = remove_quotes(split[i]);
			result->type = COMMAND_ARG_NODE;
			node->right = result;
			node = node->right;
		}
	}
	node->right = (t_ast_node *)original_right;
	return (true);
}

void	word_splitting(t_ast_node *node, t_expander *e, char *original_data)
{
	char	**split;
	char	*expanded_data;

	expanded_data = x_strdup(node->data);
	remove_null_argument(node->data);
	split = split_by_space_skip_quotes(node->data, " \t\n");
	if (!split_arg_node(split, node, expanded_data))
	{
		free_2d_array((void ***)&split);
		expand_redirect_error(original_data, e);
	}
	free(expanded_data);
	free(split);
}
