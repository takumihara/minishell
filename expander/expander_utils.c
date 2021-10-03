#include "expander.h"

bool	is_expandable_string(char *str, char delimiter)
{
	if (ft_strchr(str, delimiter))
		return (true);
	return (false);
}
