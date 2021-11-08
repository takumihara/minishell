#include "expander_internal.h"

static bool	match_mid(char *d_name, char *mid_start,
	size_t mid_len, size_t *name_pos)
{
	char	*mid_pattern;
	bool	match_mid_pattern;
	char	*pattern_start;

	match_mid_pattern = true;
	mid_pattern = x_strndup(mid_start, mid_len);
	mid_pattern = remove_quotes(mid_pattern);
	pattern_start = ft_strstr(d_name, mid_pattern);
	if (!pattern_start)
		match_mid_pattern = false;
	else
		*name_pos += pattern_start - d_name + ft_strlen(mid_pattern);
	free(mid_pattern);
	return (match_mid_pattern);
}

static bool
	match_post(char *data, char *d_name)
{
	const char		*post_data_start = strrchr_skip_quotes(data, '*') + 1;
	const size_t	post_data_len = ft_strlen(post_data_start);
	const size_t	post_d_name_len = ft_strlen(d_name);

	if (post_data_len > post_d_name_len)
		return (false);
	return (strncmp_skip_quotes(post_data_start,
			ft_strchr(d_name, 0) - unquoted_strlen(post_data_start),
			post_data_len));
}

static bool	match_pattern(char *data, char *d_name, size_t name_pos)
{
	const bool	pre_matched = strncmp_skip_quotes(data, d_name, name_pos);
	size_t		i;
	size_t		last_star_pos;
	int			status;

	if (!pre_matched)
		return (false);
	status = OUTSIDE;
	i = name_pos;
	last_star_pos = name_pos;
	while (data[i++])
	{
		status = quotation_status(data[i], status);
		if (data[i] == '*' && status == OUTSIDE)
		{
			if (!match_mid(&d_name[name_pos], &data[last_star_pos + 1],
					i - last_star_pos - 1, &name_pos))
				return (false);
			last_star_pos = i;
		}
	}
	return (match_post(data, &d_name[name_pos]));
}

void	expand_matching_pattern(t_ast_node *node, char *data,
			size_t pre_len, t_ast_node *tmp_right)
{
	DIR				*dir;
	struct dirent	*dp;
	t_wildcard		*pattern_list;

	pattern_list = NULL;
	dir = x_opendir(".");
	while (1)
	{
		dp = x_readdir(dir);
		if (!dp)
			break ;
		if (!ft_strncmp(dp->d_name, ".", 1) || !ft_strncmp(dp->d_name, "..", 2))
			if (!is_specified_dot_files(dp->d_name, pre_len, data))
				continue ;
		if (match_pattern(data, dp->d_name, pre_len))
			append_pattern_list(&pattern_list, dp->d_name);
	}
	x_closedir(dir);
	if (!pattern_list)
		return ;
	sort_pattern(pattern_list);
	expand_node_from_pattern_list(node, tmp_right, pattern_list);
	delete_pattern_list(pattern_list);
}
