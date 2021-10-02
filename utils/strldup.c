#include "utils.h"

char	*strldup(const char *str, size_t size)
{
	char	*rtn;
	size_t	i;

	if (ft_strlen(str) < size)
		size = ft_strlen(str);
	rtn = (char *)malloc(sizeof(char) * (size + 1));
	if (!rtn)
		return (NULL);
	i = 0;
	while (i < size)
	{
		rtn[i] = str[i];
		i++;
	}
	rtn[i] = '\0';
	return (rtn);
}
