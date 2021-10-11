#include "expander.h"

t_ast_node	*search_command_arg_node(t_expander *e, t_ast_node *node);
char		*expand_word(t_expander *e, char delimiter);
char		*expand_quotes_string(char *data, size_t replace_start, char quote_type);
char		*expand_environment_variable(char *data, size_t replace_starts, t_expander *e);
char		*expand_wildcard(char *data, size_t pre_len, t_expander *e);
t_ast_node	*word_splitting(t_ast_node *node, t_expander *e, char *original_data);
char		*remove_quotes(char *data, t_expander *e);

// todo: expander in execute
// t_ast_node	*expand(t_expander *e, t_env_var *env_vars, int exit_status)
t_ast_node	*expand(t_ast_node *root, t_env_var *env_vars)
{
	t_expander	*e;

	if (!root)
		return (NULL);
	if (!new_expander(&e, root, env_vars))
		exit(expand_perror(NULL, "malloc"));
	if (!search_command_arg_node(e, root))
	{
		free(e);
		return (NULL);
	}
	free(e);
	return (root);
}

t_ast_node	*search_command_arg_node(t_expander *e, t_ast_node *node)
{
	char	*original_data;

	if (!node)
		return (e->node);
	if (!search_command_arg_node(e, node->right)
		|| !search_command_arg_node(e, node->left))
		return (NULL);
	if (node->type != COMMAND_ARG_NODE && node->type != REDIRECT_IN_NODE
		&& node->type != REDIRECT_OUT_NODE && node->type != REDIRECT_APPEND_NODE)
		return (node);
	original_data = ft_strdup(node->data);
	if (!original_data)
		exit(expand_perror(e, "malloc"));
	e->node = node;
	node->data = expand_word(e, '$');
	node->data = expand_word(e, '*');
	node = word_splitting(node, e, original_data);
	if (!node)
		return (NULL);
	free(original_data);
	node->data = remove_quotes(node->data, e);
	return (node);
}

char	*expand_word(t_expander *e, char delimiter)
{
	char	*data;
	size_t	i;
	size_t	double_quote;
	size_t	single_quote;

	data = e->node->data;
	if (!data)
		return (NULL);
	if (!is_expandable_string(data, delimiter))
		return (data);
	i = 0;
	double_quote = 0;
	single_quote = 0;
	while (data[i])
	{
		if (in_quotes_type(data[i], single_quote) == DOUBLE_QUOTE)
			double_quote++;
		else if (in_quotes_type(data[i], double_quote) == SINGLE_QUOTE)
			single_quote++;
		if (data[i] == '$' && single_quote % 2 == 0 && delimiter == '$')
			data = expand_environment_variable(data, i, e);
		else if (data[i] == '*' && double_quote % 2 == 0 && single_quote % 2 == 0 && delimiter == '*')
			data = expand_wildcard(data, i, e);
		if (!data)
			return (NULL);
		if (!data[i])
			break ;
		i++;
	}
	return (data);
}

// todo: $? expands exit status
char	*expand_environment_variable(char *data, size_t replace_start, t_expander *e)
{
	const char		*var_start = &data[replace_start + 1];
	const size_t	var_len = var_strlen(var_start);
	char			*key;
	char			*value;

	key = malloc(sizeof(char) * (var_len + 1));
	if (!key)
		exit(expand_perror(e, "malloc"));
	ft_memmove(key, var_start, var_len);
	key[var_len] = '\0';
	value = get_env_value(key, e->env_vars);
	free(key);
	if (value)
		return (str_insert(data, replace_start, value, ft_strlen(value)));
	else
		return (str_insert(data, replace_start, "", 0));
}

char	*expand_wildcard(char *data, size_t pre_len, t_expander *e)
{
	DIR				*dir;
	struct dirent	*dp;
	const char		*post_start = &data[pre_len + 1];
	const size_t	post_len = unquoted_strlen(post_start);
	char			*rtn;

	dir = opendir(".");
	if (!dir)
		exit(expand_perror(e, "opendir"));
	rtn = data;
	while (1)
	{
		dp = readdir(dir);
		if (!rtn || !dp)
			break ;
		if (!ft_strncmp(dp->d_name, ".", 1))
			continue ;
		if (is_match_pattern(rtn, pre_len, dp->d_name)
			&& is_match_pattern(post_start, post_len, ft_strchr(dp->d_name, 0) - post_len))
			rtn = append_wildcard_strings(rtn, dp->d_name, data, e);
	}
	closedir(dir);
	if (rtn != data)
	{
		free(data);
		rtn = sort_strings(rtn, e);
	}
	return (rtn);
}

t_ast_node	*word_splitting(t_ast_node *node, t_expander *e, char *original_data)
{
	char		**split;
	size_t		i;
	t_ast_node	*root;
	t_ast_node	*result;
	t_ast_node	*original_right;

	if (!node->data)
		return (NULL);
	if (!*node->data && *original_data && node->type != COMMAND_ARG_NODE)
		return (expand_redirect_error(original_data));
	if (!*node->data)
		return (node);
	split = split_by_space_skip_quotes(node->data, " \t\n");
	if (!split)
		exit(expand_perror(e, "malloc"));
	free(node->data);
	i = 0;
	root = node;
	original_right = root->right;
	while (split[i])
	{
		if (i == 0)
		{
			node->data = split[i];
			if (node->type != COMMAND_ARG_NODE && ft_strcmp(node->data, original_data))
				return (expand_redirect_error(original_data));
		}
		else
		{
			if (!new_ast_node(&result))
				exit(expand_perror(e, "malloc"));
			result->data = split[i];
			result->type = COMMAND_ARG_NODE;
			node->right = result;
			node = node->right;
		}
		i++;
	}
	node->right = original_right;
	return (root);
}

char	*remove_quotes(char *data, t_expander *e)
{
	size_t	unquoted_len;
	char	*unquoted_str;

	if (!is_contain_quotes(data))
		return (data);
	unquoted_len = unquoted_strlen(data);
	unquoted_str = malloc(sizeof(char) * (unquoted_len + 1));
	if (!unquoted_str)
		exit(expand_perror(e, "malloc"));
	unquoted_str = unquoted_memmove(unquoted_str, data);
	free(data);
	data = unquoted_str;
	return (data);
}