#include "utils.h"

char	*ft_strstr(const char *hay, const char *needle)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (needle[0] == '\0')
		return ((char *)hay);
	while (hay[i])
	{
		j = 0;
		while (hay[i + j] == needle[j] && hay[i + j])
			j++;
		if (needle[j] == '\0')
			return ((char *)&hay[i]);
		i++;
	}
	return (NULL);
}
