#include "expander.h"

size_t	var_strlen(const char *str)
{
	size_t	len;

	len = 0;
	if (*str == '?')
		return (1);
	while (!ft_strchr(EXPANSION_DELIMITER, str[len]))
		len++;
	return (len);
}

char	*str_insert(char *data, size_t replace_start, char *env_value, size_t env_value_len)
{
	const size_t	origin_len = ft_strlen(data);
	const size_t	env_var_len = var_strlen(&data[replace_start + 1]) + 1;
	const size_t	expansion_len = origin_len - env_var_len + env_value_len;
	char			*prev_data;
	char			*expanded_str;

	prev_data = data;
	expanded_str = x_malloc(sizeof(*expanded_str) * (expansion_len + 1));
	ft_memmove(&expanded_str[0], &data[0], replace_start);
	ft_memmove(&expanded_str[replace_start], env_value, env_value_len);
	ft_memmove(&expanded_str[replace_start + env_value_len],
		&data[replace_start + env_var_len], origin_len - replace_start - env_var_len);
	data = expanded_str;
	free(prev_data);
	expanded_str[expansion_len] = '\0';
	return (data);
}

bool	is_expandable_env_var(char start, int status)
{
	if (status == OUTSIDE)
	{
		if (start == '\0')
			return (false);
	}
	else if (status == IN_DOUBLE_QUOTE)
	{
		if (start == '\'' || start == '\"')
			return (false);
	}
	return (true);
}
