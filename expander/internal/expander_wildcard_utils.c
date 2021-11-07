#include "expander_internal.h"

bool	is_specified_dot_files(char *d_name, size_t len, char *data)
{
	bool	valid_dot;
	char	*data_copy;
	char	*unquoted_data;

	valid_dot = false;
	data_copy = x_strndup(data, len);
	unquoted_data = remove_quotes(data_copy);
	if (!ft_strcmp(unquoted_data, "."))
		valid_dot = true;
	if (!ft_strncmp(d_name, "..", 2) && !ft_strcmp(unquoted_data, ".."))
		valid_dot = true;
	free(unquoted_data);
	return (valid_dot);
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

char	*strrchr_skip_quotes(char *data, int c)
{
	int	i;
	int	status;

	i = 0;
	status = OUTSIDE;
	while (data[i])
		i++;
	while (i >= 0)
	{
		status = quotation_status(data[i], status);
		if (data[i] == (char)c && status == OUTSIDE)
			return ((char *)&data[i]);
		i--;
	}
	return (NULL);
}

bool	strncmp_skip_quotes(const char *data, char *d_name, size_t len)
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
