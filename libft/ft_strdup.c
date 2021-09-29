#include "libft.h"

char	*ft_strdup(const char *str)
{
	char	*rtn;
	size_t	i;

	rtn = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!rtn)
		return ((void *)0);
	i = 0;
	while (str[i])
	{
		rtn[i] = str[i];
		i++;
	}
	rtn[i] = '\0';
	return (rtn);
}
