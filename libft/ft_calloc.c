#include <printf.h>
#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void		*buffer;

	if (size != 0 && count > SIZE_MAX / size)
		return (NULL);
	buffer = (void *)malloc(count * size);
	if (!buffer)
		return (NULL);
	if (count * size == 0)
		ft_bzero(buffer, 1);
	else
		ft_bzero(buffer, count * size);
	return (buffer);
}
