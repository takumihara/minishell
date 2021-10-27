#include "../libft/libft.h"
#include "../utils/utils.h"

char	*x_substr(char const *s, unsigned int start, size_t len)
{
	char	*res;

	res = ft_substr(s, start, len);
	if (!res)
		perror_exit("malloc", EXIT_FAILURE);
	return (res);
}
