#include "expander_internal.h"

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

char	*str_insert(char *data, size_t replace_start,
				char *env_value, size_t env_value_len)
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
		&data[replace_start + env_var_len],
		origin_len - replace_start - env_var_len);
	data = expanded_str;
	free(prev_data);
	expanded_str[expansion_len] = '\0';
	return (data);
}

bool	is_expandable_env_var(char start, int status)
{
	if (ft_isspace(start) || start == '\0')
		return (false);
	else if (status == IN_DOUBLE_QUOTE && (start == '\'' || start == '\"'))
		return (false);
	return (true);
}

char	*expand_environment_variable(char *data, size_t replace_start,
	t_expander *e, int status)
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
		key = x_malloc(sizeof(*key) * (var_len + 1));
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
