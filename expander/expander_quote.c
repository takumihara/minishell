#include "expander.h"

bool	is_quoted(const char *str)
{
	return (ft_strchr(str, '\'') || ft_strchr(str, '\"'));
}

size_t	unquoted_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (*str)
	{
		if (*str != '\'' && *str != '\"')
			len++;
		str++;
	}
	return (len);
}
