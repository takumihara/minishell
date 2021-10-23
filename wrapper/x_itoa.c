#include "x.h"

char	*x_itoa(int n)
{
	char	*res;

	res = ft_itoa(n);
	if (!res)
		perror_exit("malloc", EXIT_FAILURE);
	return (res);
}
