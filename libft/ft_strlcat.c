#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	len;

	len = 0;
	while (*dst)
	{
		dst++;
		len++;
	}
	if (len < size)
	{
		while (*src && len + 1 < size)
		{
			*dst++ = *src++;
			len++;
		}
		*dst = '\0';
	}
	else
		len = size;
	while (*src++)
		len++;
	return (len);
}
