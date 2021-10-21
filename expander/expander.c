#include <dirent.h>
#include "expander.h"

void		search_command_arg_node(t_expander *e, t_ast_node *node);
char		*expand_word(t_expander *e, char delimiter);
char		*expand_quotes_string(char *data, size_t replace_start, char quote_type);
char		*expand_environment_variable(char *data, size_t replace_starts, t_expander *e, int status);
char		*expand_wildcard(char *data, size_t pre_len, t_expander *e);
t_ast_node	*word_splitting(t_ast_node *node, t_expander *e, char *original_data);
char		*remove_quotes(char *data, t_expander *e);

// todo: expander in execute
t_ast_node	*expand(t_ast_node *root, t_env_var **env_vars, int exit_status)
{
	t_expander	*e;

	(void)exit_status;
	if (!root)
		return (NULL);
	new_expander(&e, root, *env_vars);
	search_command_arg_node(e, root);
	free(e);
	return (root);
}

void search_command_arg_node(t_expander *e, t_ast_node *node)
{
	char		*original_data;
	t_ast_node	*head;

	head = node;
	if (!node)
		return ;
	search_command_arg_node(e, node->right);
	search_command_arg_node(e, node->left);
	if (node->type != COMMAND_ARG_NODE && node->type != REDIRECT_IN_NODE
		&& node->type != REDIRECT_OUT_NODE && node->type != REDIRECT_APPEND_NODE)
		return ;
	original_data = x_strdup(node->data);
	e->node = node;
	node->data = expand_word(e, '$');
	node->data = expand_word(e, '*');
	node = word_splitting(node, e, original_data);
	if (!node)
		return ;
	free(original_data);
	// return (head);
}

char	*expand_word(t_expander *e, char delimiter)
{
	char	*data;
	int		status;
	size_t	i;

	data = e->node->data;
	if (!data)
		return (NULL);
	i = 0;
	status = OUTSIDE;
	while (data[i])
	{
		status = quotation_status(data[i], status);
		if (data[i] == '$' && delimiter == '$' && status != IN_SINGLE_QUOTE)
			data = expand_environment_variable(data, i, e, status);
		else if (data[i] == '*' && delimiter == '*' && status == OUTSIDE)
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
		key = malloc(sizeof(char) * (var_len + 1));
		if (!key)
			exit(expand_perror(e, "malloc"));
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
	remove_null_argument(node->data);
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
			node->data = remove_quotes(split[i], e);
		}
		else
		{
			new_ast_node(&result);
			result->data = remove_quotes(split[i], e);
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

	if (!contain_quotes(data))
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