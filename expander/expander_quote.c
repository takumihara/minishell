#include "expander.h"

bool	is_quoted(const char *str)
{
	if (ft_strchr(str, '\'') || ft_strchr(str, '\"'))
		return (true);
	return (false);
}
