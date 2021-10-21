#include <dirent.h>
#include "expander.h"

void		search_command_arg_node(t_expander *e, t_ast_node *node);
char		*expand_word(t_expander *e, char *data, char delimiter);
char		*expand_quotes_string(char *data, size_t replace_start, char quote_type);
char		*expand_environment_variable(char *data, size_t replace_starts, t_expander *e, int status);
char		*expand_wildcard(char *data, size_t pre_len);
t_ast_node	*word_splitting(t_ast_node *node, t_expander *e, char *original_data);
char		*remove_quotes(char *data);

void	print_node(t_ast_node *root)
{
	while (root)
	{
		printf("type: %d, data:%s\n", root->type, root->data);
		root = root->right;
	}
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
	// print_node(root);
	return (root);
}

void search_command_arg_node(t_expander *e, t_ast_node *node)
{
	char		*original_data;
	
	if (!node)
		return ;
	search_command_arg_node(e, node->right);
	search_command_arg_node(e, node->left);
	if (node->type != COMMAND_ARG_NODE && node->type != REDIRECT_IN_NODE
		&& node->type != REDIRECT_OUT_NODE && node->type != REDIRECT_APPEND_NODE)
		return ;
	original_data = x_strdup(node->data);
	node->data = expand_word(e, node->data, '$');
	node->data = expand_word(e, node->data, '*');
	node = word_splitting(node, e, original_data);
	free(original_data);
}

char	*expand_word(t_expander *e, char *data, char delimiter)
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

char	*expand_environment_variable(char *data, size_t replace_start, t_expander *e, int status)
{
	const char		*var_start = &data[replace_start + 1];
	const size_t	var_len = var_strlen(var_start);
	char			*key;
	char			*value;

	if (!is_expandable_env_var(*var_start, status))
		return (data);
	if (*var_start == '?')
		value = get_env_value("?", e->env_vars);
	else
	{
		key = x_malloc(sizeof(*key) * (var_len + 1));
		ft_memmove(key, var_start, var_len);
		key[var_len] = '\0';
		value = get_env_value(key, e->env_vars);
		free(key);
	}
	if (value)
		return (str_insert(data, replace_start, value, ft_strlen(value)));
	else
		return (str_insert(data, replace_start, "", 0));
}

char	*expand_wildcard(char *data, size_t pre_len)
{
	DIR				*dir;
	struct dirent	*dp;
	const char		*post_start = &data[pre_len + 1];
	const size_t	post_len = unquoted_strlen(post_start);
	char			*rtn;

	dir = x_opendir(".");
	rtn = data;
	while (1)
	{
		dp = x_readdir(dir);
		if (!dp)
			break ;
		if (!ft_strncmp(dp->d_name, ".", 1))
			continue ;
		if (is_match_pattern(rtn, pre_len, dp->d_name)
			&& is_match_pattern(post_start, post_len, ft_strchr(dp->d_name, 0) - post_len))
			rtn = append_wildcard_strings(rtn, dp->d_name, data);
	}
	x_closedir(dir);
	if (rtn != data)
		rtn = sort_strings(rtn, data);
	return (rtn);
}

t_ast_node	*word_splitting(t_ast_node *node, t_expander *e, char *original_data)
{
	char		**split;

	if (!*node->data && *original_data && node->type != COMMAND_ARG_NODE)
		return (expand_redirect_error(original_data, node, e));
	if (!*node->data)
		return (node);
	remove_null_argument(node->data);
	split = split_by_space_skip_quotes(node->data, " \t\n");
	free(node->data);
	node = split_arg_node(split, node, original_data, e);
	return (node);
}

char	*remove_quotes(char *data)
{
	size_t	unquoted_len;
	char	*unquoted_str;

	if (!contain_quotes(data))
		return (data);
	unquoted_len = unquoted_strlen(data);
	unquoted_str = x_malloc(sizeof(*unquoted_str) * (unquoted_len + 1));
	unquoted_str = unquoted_memmove(unquoted_str, data);
	free(data);
	return (unquoted_str);
}