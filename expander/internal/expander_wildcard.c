#include "expander_internal.h"

static bool	find_mid_str(char *d_name, char *mid_start, size_t mid_len, size_t *name_pos)
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

static bool	match_mid(char *data, char *d_name, size_t name_pos)
{
	size_t		i;
	size_t		len;
	int			status;
	int			star_count;

	status = OUTSIDE;
	star_count = 1;
	i = name_pos;
	len = name_pos;
	while (data[i++])
	{
		status = quotation_status(data[i], status);
		if (data[i] == '*' && status == OUTSIDE)
		{
			if (!find_mid_str(&d_name[name_pos], &data[len + star_count],
					i - len - star_count, &name_pos))
				return (false);
			len = i - star_count;
			star_count++;
		}
	}
	return (true);
}

static bool
	match_pre_post(char *data, char *d_name, size_t pre_len, size_t post_len)
{
	const char	*post_start = strrchr_skip_quotes(data, '*') + 1;

	return (strncmp_skip_quotes(data, d_name, pre_len)
		&& strncmp_skip_quotes(post_start,
			ft_strchr(d_name, 0) - unquoted_strlen(post_start), post_len));
}

static char	*expand_matching_pattern(char *data, size_t pre_len)
{
	DIR				*dir;
	struct dirent	*dp;
	char			*rtn;
	const size_t	post_len = ft_strlen(strrchr_skip_quotes(data, '*') + 1);

	dir = x_opendir(".");
	rtn = data;
	while (1)
	{
		dp = x_readdir(dir);
		if (!dp)
			break ;
		if (!ft_strncmp(dp->d_name, ".", 1) || !ft_strncmp(dp->d_name, "..", 2))
			if (is_not_printable_dot_files(dp->d_name, pre_len, data))
				continue ;
		if (match_pre_post(data, dp->d_name, pre_len, post_len)
			&& match_mid(data, dp->d_name, pre_len))
			rtn = append_wildcard_strings(rtn, dp->d_name, data);
	}
	x_closedir(dir);
	if (rtn != data)
		rtn = sort_strings(rtn, data);
	return (rtn);
}

char	*expand_wildcard(char *data, size_t pre_len)
{
	char	*original_data;
	bool	pattern_stars;

	original_data = x_strdup(data);
	data = remove_multi_stars(data);
	pattern_stars = contain_stars_as_pattern(data);
	data = expand_matching_pattern(data, pre_len);
	if (!pattern_stars && ft_strchr(data, '*'))
	{
		free(data);
		return (original_data);
	}
	else
	{
		free(original_data);
		return (data);
	}
}
