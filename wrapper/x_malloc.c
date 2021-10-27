#include <stdlib.h>

#include "../utils/utils.h"

void	*x_malloc(size_t size)
{
	void	*res;

	res = malloc(size);
	if (!res)
		perror_exit("malloc", EXIT_FAILURE);
	return (res);
}
