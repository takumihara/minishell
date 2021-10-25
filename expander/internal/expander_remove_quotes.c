#include "expander_internal.h"

static bool	contain_quotes(const char *str)
{
	return (ft_strchr(str, '\'') || ft_strchr(str, '\"'));
}

static char	*unquoted_memmove(char *dst, char *src)
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

char	*remove_quotes(char *data)
{
	size_t	unquoted_len;
	char	*unquoted_str;

	if (!contain_quotes(data))
		return (data);
	unquoted_len = unquoted_strlen(data);
	unquoted_str = x_malloc(sizeof(*unquoted_str) * (unquoted_len + 1));
	unquoted_str = unquoted_memmove(unquoted_str, data);
	free(data);
	return (unquoted_str);
}
