#include "libft.h"

char	*ft_strappend(char *dst, const char *src, size_t l)
{
	char	*rtn;
	size_t	i;
	size_t	j;
	size_t	k;

	rtn = (char *)malloc(sizeof(char) * (ft_strlen(dst) + l + 1));
	if (!rtn)
	{
		free(dst);
		return (NULL);
	}
	i = 0;
	j = 0;
	while (dst[j])
		rtn[i++] = dst[j++];
	k = 0;
	while (k < l)
		rtn[i++] = src[k++];
	rtn[i] = '\0';
	free(dst);
	return (rtn);
}