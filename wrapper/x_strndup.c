#include "x.h"

char	*x_strndup(const char *str, size_t size)
{
	char	*res;

	res = ft_strndup(str, size);
	if (!res)
		perror_exit("malloc", EXIT_FAILURE);
	return (res);
}
