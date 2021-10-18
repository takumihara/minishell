#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	char	*rtn;
	size_t	i;

	if (!s1 || !s2)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	rtn = (char *)malloc(sizeof(char) * (len_s1 + len_s2 + 1));
	if (!rtn)
		return ((void *)0);
	i = 0;
	len_s1 = 0;
	while (s1[len_s1])
		rtn[i++] = s1[len_s1++];
	len_s2 = 0;
	while (s2[len_s2])
		rtn[i++] = s2[len_s2++];
	rtn[i] = '\0';
	return (rtn);
}
