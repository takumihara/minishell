#include "expander_internal.h"


// char	*remove_multiple_stars(char *data, size_t len)
// {
// 	size_t	i;

// 	i = 0;
// 	while (1)
// 		;
// }

// bool	contain_multiple_stars(char *data, size_t *i)
// {

// }

size_t	pre_star_strlen(char *data, size_t pre_len)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = 0;
	while (i < pre_len)
	{
		if (data[i] != '*')
			len++;
		i++;
	}
	return (len);
}

bool	pattern_matching(char *data, char *d_name, size_t len)
{
	size_t	i;
	size_t	j;
	int		status;

	i = 0;
	j = 0;
	status = OUTSIDE;
	while (i < len)
	{
		status = quotation_status(data[i], status);
		if ((data[i] == '\'' && status != IN_DOUBLE_QUOTE)
			|| (data[i] == '\"' && status != IN_SINGLE_QUOTE))
		{
			i++;
			continue ;
		}
		if (data[i++] != d_name[j++])
			return (false);
	}
	return (true);
}

bool	find_str_skip_quotes(char *d_name, char *mid_str)
{
	bool	match_str;

	match_str = true;
	mid_str = remove_quotes(mid_str);
	if (!ft_strstr(d_name, mid_str))
		match_str = false;
	free(mid_str);
	return (match_str);
}

bool	is_match_pattern(char *data, char *d_name, size_t pre_len, size_t post_len)
{
	size_t	i;
	size_t	len;
	int		status;
	int		count;
	char	*mid_str;

	i = 0;
	status = OUTSIDE;
	count = 0;
	while (1)
	{
		status = quotation_status(data[i], status);
		if (data[i] == '*' && status == OUTSIDE)
		{
			if (count == 0)
			{
				if (!pattern_matching(data, d_name, pre_len))
					return (false);
			}
			else
			{
				mid_str = x_strndup(&data[len + count], i - len - count);
				if (!find_str_skip_quotes(&d_name[len], mid_str))
					return (false);
			}
			len = pre_star_strlen(data, i);
			i++;
			count++;
			continue ;
		}
		else if (data[i] == '\0')
		{

			if (!pattern_matching(strrchr_skip_quotes(data, '*') + 1,
				ft_strchr(d_name, 0) - unquoted_strlen(strrchr_skip_quotes(data, '*') + 1), post_len))
					return (false);
			break ;
		}
		i++;
	}
	return (true);
}

char	*search_matching_pattern(char *data, size_t pre_len)
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
		// printf("d_name[%s]\n", dp->d_name);
		if (!ft_strncmp(dp->d_name, ".", 1) || !ft_strncmp(dp->d_name, "..", 2))
			if (is_not_printable_dot_files(dp->d_name, pre_len, data))
				continue ;
		if (is_match_pattern(data, dp->d_name, pre_len, post_len))
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
	// size_t	i;

	original_data = x_strdup(data);
	// wildcard proccess
	// while (contain_multiple_stars(data, &i))
	// 	data = remove_multiple_stars(data, i);
	data = search_matching_pattern(data, pre_len);
	if (data[pre_len] == '*')
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
