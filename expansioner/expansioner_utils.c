#include "expansioner.h"

bool	is_expandable_string(char *str, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		if (ft_strchr(EXPANDABLE, *str))
			return (true);
		str++;
		i++;
	}
	return (false);
}

size_t	var_len(const char *str)
{
	size_t	len;

	len = 0;
	while (!ft_strchr(EXPANSION_DELIMITER, str[len]))
		len++;
	return (len);
}

char	*search_env_vars(t_string *data, size_t var_start, t_env_var *vars)
{
	const char	*start = &data->start[var_start];
	size_t		len;

	len = var_len(start);
	while (vars && ft_strncmp(vars->key, start, len))
		vars = vars->next;
	if (vars)
		return (vars->value);
	else
		return (NULL);
}

t_string	*str_insert(t_string *data, char *env_value, size_t replace_start)
{
	const size_t	origin_len = data->len;
	const size_t	env_var_len = var_len(&data->start[replace_start + 1]) + 1;
	const size_t	env_value_len = ft_strlen(env_value);
	const size_t	expanded_len = origin_len - env_var_len + env_value_len;
	char			*expanded_str;

	expanded_str = (char *)malloc(sizeof(char) * (expanded_len + 1));
	if (!expanded_str)
		return (NULL);
	ft_memmove(&expanded_str[0], &data->start[0], replace_start);
	ft_memmove(&expanded_str[replace_start], env_value, env_value_len);
	ft_memmove(&expanded_str[replace_start + env_value_len],
		&data->start[replace_start + env_var_len], origin_len - replace_start - env_var_len);
	data->start = expanded_str;
	data->len = expanded_len;
	expanded_str[expanded_len] = '\0';
	return (data);
}
