#include "libft.h"
#include <libc.h>

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*uc_dst;
	unsigned const char	*uc_src;

	uc_dst = dst;
	uc_src = src;
	if (dst < src)
		while (0 < len--)
			*uc_dst++ = *uc_src++;
	else if (dst > src)
		while (0 < len--)
			uc_dst[len] = uc_src[len];
	return (dst);
}
