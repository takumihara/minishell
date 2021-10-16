#include "utils.h"

bool	atoi_strict(const char *str, int *num)
{
	long long	converted;
	int			sign;

	converted = 0;
	sign = 1;
	while (('\t' <= *str && *str <= '\r') || *str == ' ')
		str++;
	if ((*str == '+' || *str == '-') && *str++ == '-')
		sign = -1;
	if (*str == '\0')
		return (false);
	while (ft_isdigit(*str))
	{
		if ((sign * converted * 10 + sign * (*str - '0')) / 10 != sign * converted)
			return (false);
		converted = converted * 10 + *str - '0';
		str++;
	}
	if (*str == '\0')
	{
		*num = (int)(sign * converted);
		return (true);
	}
	return (false);
}
