#include "../libft/libft.h"
#include "../utils/utils.h"

char	*x_strdup(const char *str)
{
	char	*res;

	res = ft_strdup(str);
	if (!res)
		perror_exit("malloc", EXIT_FAILURE);
	return (res);
}