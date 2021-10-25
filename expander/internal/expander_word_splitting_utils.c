#include "expander_internal.h"

size_t	skip_quotes(const char *str, char quote_type)
{
	size_t	len;

	len = 1;
	while (str[len] && str[len] != quote_type)
		len++;
	return (len);
}

bool	is_delims(char c, char const *delims)
{
	size_t	j;

	j = 0;
	while (delims[j])
	{
		if (delims[j] == c)
			return (true);
		j++;
	}
	return (false);
}
