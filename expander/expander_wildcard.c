#include "expander.h"

char	*append_wildcard_strings(char *dst, const char *src, size_t count)
{
	if (count == 0)
	{
		dst = ft_strdup(src);
		return (dst);
	}
	else
	{
		dst = strappend(dst, " ", 1);
		if (!dst)
			return (NULL);
		dst = strappend(dst, src, ft_strlen(src));
		return (dst);
	}
}

bool	is_match_pattern(const char *data, size_t len, char *name)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (i < len)
	{
		if (data[i] == '\"' || data[i] == '\'')
		{
			i++;
			continue ;
		}
		if (data[i++] != name[j++])
			return (false);
	}
	return (true);
}
