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
