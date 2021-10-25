#include "expander_internal.h"

char	*append_wildcard_strings(char *dst, char *src, const char *data)
{
	if (dst == data)
		return (x_strdup(src));
	else
	{
		dst = strappend(dst, " ", 1);
		dst = strappend(dst, src, ft_strlen(src));
		return (dst);
	}
}

bool	is_match_pattern(const char *data, size_t len, char *name)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (i < len)
	{
		if (data[i] == '\"' || data[i] == '\'')
		{	
			i++;
			continue ;
		}
		if (data[i++] != name[j++])
			return (false);
	}
	return (true);
}

char	*sort_strings(char *src, char *data)
{
	char	**wildcard_array;
	size_t	word_num;
	char	*rtn;
	size_t	i;

	free(data);
	wildcard_array = x_split(src, ' ');
	free(src);
	word_num = 0;
	while (wildcard_array[word_num])
		word_num++;
	quick_sort_str(wildcard_array, 0, word_num - 1);
	rtn = NULL;
	i = 0;
	while (i < word_num)
		rtn = append_wildcard_strings(rtn, wildcard_array[i++], NULL);
	free_2d_array((void ***)&wildcard_array);
	return (rtn);
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
			&& is_match_pattern(post_start, post_len,
				ft_strchr(dp->d_name, 0) - post_len))
			rtn = append_wildcard_strings(rtn, dp->d_name, data);
	}
	x_closedir(dir);
	if (rtn != data)
		rtn = sort_strings(rtn, data);
	return (rtn);
}
