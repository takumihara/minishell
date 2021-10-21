#include "x.h"

char	**x_split(const char *s, char c)
{
	char	**split;

	split = ft_split(s, c);
	if (!split)
		perror_exit("malloc", EXIT_FAILURE);
	return (split);
}
