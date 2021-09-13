#include "libft.h"

int	ft_strncmp(const char *str1, const char *str2, size_t n)
{
	const unsigned char	*uc_str1;
	const unsigned char	*uc_str2;

	uc_str1 = (const unsigned char *)str1;
	uc_str2 = (const unsigned char *)str2;
	while (*uc_str1 && *uc_str2 && *uc_str1 == *uc_str2 && 0 < n)
	{
		uc_str1++;
		uc_str2++;
		n--;
	}
	if (n == 0)
		return (0);
	else
		return (*uc_str1 - *uc_str2);
}
