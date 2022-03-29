#include "expander_internal.h"

bool	is_expanded_data(char *pre_expansion_data, char *data)
{
	bool	expanded;

	expanded = false;
	if (ft_strcmp(pre_expansion_data, data))
		expanded = true;
	free(pre_expansion_data);
	return (expanded);
}

bool	is_expandable_env_var(char start, int status)
{
	if (ft_isspace(start) || start == '\0')
		return (false);
	else if (status == IN_DOUBLE_QUOTE && (start == '\'' || start == '\"'))
		return (false);
	return (true);
}

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
