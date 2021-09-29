#include "../libft/libft.h"

bool	assign_mem(void **dst, void *src)
{
	if (!src)
		return (false);
	*dst = src;
	return (true);
}
