#include "libft.h"

int	ft_isalnum(char c)
{
	return (ft_isdigit(c) || ft_isalpha(c));
}
