#include "expander.h"

bool	is_quote(const char c)
{
	return (c == '\'' || c == '\"');
}

size_t	unquoted_strlen(const char *str)
{
	size_t	len;
	size_t	single_quote;
	size_t	double_quote;

	len = 0;
	single_quote = 0;
	double_quote = 0;
	while (*str)
	{
		if (*str == '\"' && single_quote % 2 == 0)
			double_quote++;
		else if (*str == '\'' && double_quote % 2 == 0)
			single_quote++;
		else
			len++;
		str++;
	}
	return (len);
}

bool	is_removable_quotes(const char *str)
{
	return (ft_strchr(str, '\'') || ft_strchr(str, '\"'));
}

char	*unquoted_memmove(char *dst, char *src)
{
	size_t	single_quote;
	size_t	double_quote;
	char	*dst_start;

	single_quote = 0;
	double_quote = 0;
	dst_start = dst;
	while (*src)
	{
		if (*src == '\"' && single_quote % 2 == 0)
			double_quote++;
		else if (*src == '\'' && double_quote % 2 == 0)
			single_quote++;
		else
			*dst++ = *src;
		src++;
	}
	*dst = '\0';
	return (dst_start);
}
