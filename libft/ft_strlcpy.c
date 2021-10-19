#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	len;

	len = 0;
	while (src[len] && len + 1 < size)
	{
		dst[len] = src[len];
		len++;
	}
	if (size != 0)
		dst[len] = '\0';
	while (src[len])
		len++;
	return (len);
}
