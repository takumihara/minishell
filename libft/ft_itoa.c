#include "libft.h"

static char	*len_malloc(int n, int *len)
{
	char	*str;
	int		count;

	count = 1;
	if (n < 0)
		count++;
	while (n >= 10 || n <= -10)
	{
		n /= 10;
		count++;
	}
	str = malloc(sizeof(char) * (count + 1));
	if (str == NULL)
		return (NULL);
	*len = count;
	return (str);
}

char	*ft_itoa(int n)
{
	int		len;
	char	*str;

	len = 0;
	str = len_malloc(n, &len);
	if (str == NULL)
		return (NULL);
	if (n < 0)
		str[0] = '-';
	if (n == 0)
		str[0] = '0';
	str[len] = '\0';
	while (n)
	{
		if (n > 0)
			str[--len] = '0' + (n % 10);
		else
			str[--len] = '0' - (n % 10);
		n /= 10;
	}
	return (str);
}
