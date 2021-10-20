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
		if (in_quotes_type(*str, single_quote) == DOUBLE_QUOTE)
			double_quote++;
		else if (in_quotes_type(*str, double_quote) == SINGLE_QUOTE)
			single_quote++;
		else
			len++;
		str++;
	}
	return (len);
}

bool	contain_quotes(const char *str)
{
	return (ft_strchr(str, '\'') || ft_strchr(str, '\"'));
}

int		in_quotes_type(char c, size_t count)
{
	if  (c == '\"' && count % 2 == 0)
		return (DOUBLE_QUOTE);
	else if (c == '\'' && count % 2 == 0)
		return (SINGLE_QUOTE);
	return (NOT_QUOTE);
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
		if (in_quotes_type(*src, single_quote) == DOUBLE_QUOTE)
			double_quote++;
		else if (in_quotes_type(*src, double_quote) == SINGLE_QUOTE)
			single_quote++;
		else
			*dst++ = *src;
		src++;
	}
	*dst = '\0';
	return (dst_start);
}

void	remove_null_argument(char *str)
{
	int		status;
	size_t	i;

	i = 0;
	while (!ft_strncmp(&str[i], "\"\"", 2) || !ft_strncmp(&str[i], "\'\'", 2))
		i += 2;
	if (str[i] == '\0')
		return ;
	status = OUTSIDE;
	while (*str)
	{
		if (status == OUTSIDE)
		{
			if (!ft_strncmp(str, "\"\"", 2) || !ft_strncmp(str, "\'\'", 2))
			{
				ft_memmove(str, str + 2, ft_strlen(str + 2) + 1);
				continue ;
			}
		}
		status = quotation_status(*str, status);
		str++;
	}
}
