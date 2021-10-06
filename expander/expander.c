#include "expander.h"

void		search_command_arg_node(t_expander *e, t_ast_node *node);
char		*expand_word(t_expander *e, char delimiter, char *(*f)(char *, size_t, t_expander *));
char		*expand_quotes_string(char *data, size_t replace_start, char quote_type);
char		*expand_environment_variable(char *data, size_t replace_starts, t_expander *e);
char		*expand_wildcard(char *data, size_t pre_len, t_expander *e);
t_ast_node	*word_splitting(t_ast_node *node, t_expander *e);

t_ast_node	*expand(t_ast_node *root, char **environ)
{
	t_expander	*e;

	(void)environ;
	if (!root)
		return (NULL);
	if (!new_expander(&e, root))
		exit(expand_perror(NULL, "malloc"));
	search_command_arg_node(e, root);
	free(e);
	return (root);
}

void	search_command_arg_node(t_expander *e, t_ast_node *node)
{
	if (!node)
		return ;
	search_command_arg_node(e, node->right);
	search_command_arg_node(e, node->left);
	if (node->type != COMMAND_ARG_NODE)
		return ;
	e->node = node;
	// todo: export env_var
	// if (!ft_strcmp(node->data, "export"))
	// 	export_env_var();
	node->data = expand_word(e, '$', &expand_environment_variable);
	node->data = expand_word(e, '*', &expand_wildcard);
	node = word_splitting(node, e);
	// todo: remove quotes
	// data = remove_quotes();
}

char	*expand_word(t_expander *e, char delimiter, char *(*f)(char *, size_t, t_expander *))
{
	char	*data;
	size_t	i;
	size_t	double_quote;
	size_t	single_quote;

	data = e->node->data;
	printf("%s\n", data);
	if (!data)
		return (NULL);
	if (!is_expandable_string(data, delimiter))
		return (data);
	i = 0;
	double_quote = 0;
	single_quote = 0;
	while (data[i])
	{
		if (data[i] == '\"' && single_quote % 2 == 0)
			double_quote++;
		else if (data[i] == '\'' && double_quote % 2 == 0)
			single_quote++;
		else if (data[i] == delimiter && single_quote % 2 == 0)
			data = f(data, i, e);
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
	value = getenv(key);
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

t_ast_node	*word_splitting(t_ast_node *node, t_expander *e)
{
	char		**split;
	size_t		i;
	t_ast_node	*root;
	t_ast_node	*result;

	if (!node->data)
		return (NULL);
	if (!*node->data)
		return (node);
	split = split_by_delims(node->data, " \t\n");
	if (!split)
		exit(expand_perror(e, "malloc"));
	free(node->data);
	i = 0;
	root = node;
	while (split[i])
	{
		if (i == 0)
			node->data = split[i];
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
	return (root);
}
