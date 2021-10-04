#include "expander.h"

void	search_command_arg_node(t_ast_node *node, t_env_var *vars);
char	*expand_word(char *data, t_env_var *vars, char delimiter, char *(*f)(char *, size_t, t_env_var *));
char	*expand_quotes_string(char *data, size_t replace_start, t_env_var *vars, char quote_type);
char	*expand_environment_variable(char *data, size_t replace_start, t_env_var *vars);
char	*expand_wildcard(char *data, size_t pre_len, t_env_var *vars);

t_ast_node	*expand(t_ast_node *node, char **envp)
{
	t_ast_node	*root;
	t_env_var	*vars;

	vars = split_environment_vars(envp);
	if (!vars)
		return (NULL);
	// todo: remove this. To confirm env list.
	// print_env_lst(vars);
	root = node;
	search_command_arg_node(node, vars);
	env_lstclear(vars);
	return (root);
}

void	search_command_arg_node(t_ast_node *node, t_env_var *vars)
{
	if (!node)
		return ;
	search_command_arg_node(node->right, vars);
	search_command_arg_node(node->left, vars);
	if (node->type != COMMAND_ARG_NODE)
		return ;
	// todo: export env_var
	// if (!ft_strcmp(node->data, "export"))
	// 	export_env_var();
	node->data = expand_word(node->data, vars, '$', &expand_environment_variable);
	node->data = expand_word(node->data, vars, '*', &expand_wildcard);
	// todo: remove quotes
	// data = remove_quotes();
}

char	*expand_word(char *data, t_env_var *vars, char delimiter, char *(*f)(char *, size_t, t_env_var *))
{
	size_t	i;
	size_t	double_quote;
	size_t	single_quote;

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
			data = f(data, i, vars);
		if (!data)
			return (NULL);
		if (!data[i])
			break ;
		i++;
	}
	return (data);
}

// todo: $? expands exit status
char	*expand_environment_variable(char *data, size_t replace_start, t_env_var *vars)
{
	char	*env_value;

	env_value = search_env_vars(data, replace_start + 1, vars);
	if (!env_value)
		return (data);
	data = str_insert(data, replace_start, env_value, ft_strlen(env_value));
	return (data);
}

char	*expand_wildcard(char *data, size_t pre_len, t_env_var *vars)
{
	DIR				*dir;
	struct dirent	*dp;
	const char		*post_start = &data[pre_len + 1];
	const size_t	post_len = not_quoted_strlen(post_start);
	char			*rtn;

	(void)vars;
	dir = opendir(".");
	if (!dir)
		return (NULL);
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
			rtn = append_wildcard_strings(rtn, dp->d_name, data);
	}
	closedir(dir);
	if (rtn != data)
		free(data);
	return (rtn);
}
