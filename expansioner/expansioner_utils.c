#include "expansioner.h"

bool	is_expandable_string(char *str)
{
	while (*str)
	{
		if (ft_strchr(EXPANDABLE, *str))
			return (true);
		str++;
	}
	return (false);
}

char	*cut_environment_variable(char *var_start)
{
	size_t	len;
	char	*env_var;

	len = 0;
	while (!ft_strchr(EXPANSION_DELIMITER, var_start[len]))
		len++;
	env_var = (char *)malloc(sizeof(char) * (len + 1));
	if (!env_var)
		return (NULL);
	ft_memmove(env_var, var_start, len);
	env_var[len] = '\0';
	return (env_var);
}

t_string	*str_insert(t_string *data, char *env_var, char *env_value, size_t replace_start)
{
	const size_t	origin_len = data->len;
	const size_t	env_var_len = ft_strlen(env_var) + 1;
	const size_t	env_value_len = ft_strlen(env_value);
	char			*expanded;
	size_t			expanded_len = origin_len - env_var_len + env_value_len;

	expanded = (char *)malloc(sizeof(char) * (expanded_len + 1));
	if (!expanded)
		return (NULL);
	ft_memmove(&expanded[0], &data->start[0], replace_start);
	ft_memmove(&expanded[replace_start], env_value, env_value_len);
	ft_memmove(&expanded[replace_start + env_value_len],
		&env_value[replace_start + env_var_len], origin_len - replace_start - env_var_len);
	data->start = expanded;
	data->len = expanded_len;
	return (data);
}
