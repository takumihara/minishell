#include "expander.h"

bool	is_quoted(const char *str)
{
	if (ft_strchr(str, '\'') || ft_strchr(str, '\"'))
		return (true);
	return (false);
}

size_t	not_quoted_strlen(const char *str)
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
