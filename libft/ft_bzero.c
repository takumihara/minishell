#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*uc;

	uc = (unsigned char *)s;
	while (0 < n--)
		*uc++ = '\0';
}
