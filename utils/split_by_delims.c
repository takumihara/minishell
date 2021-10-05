#include "utils.h"

static bool	is_delims(char c, char const *delims)
{
	size_t	j;

	j = 0;
	while (delims[j])
	{
		if (delims[j] == c)
			return (true);
		j++;
	}
	return (false);
}

static char	**row_malloc_split(char const *str, const char *delims, size_t *row)
{
	size_t	len;
	size_t	i;
	char	**split;

	i = 0;
	len = 0;
	while (str[i])
	{
		if (!is_delims(str[i], delims))
		{
			while (!is_delims(str[i], delims) && str[i])
				i++;
			len++;
		}
		if (!str[i])
			break ;
		i++;
	}
	split = (char **)malloc(sizeof(char *) * (len + 1));
	if (split == NULL)
		return (NULL);
	split[len] = NULL;
	*row = len;
	return (split);
}

static char	*ft_strdup_split(char const *src, const char *delims)
{
	size_t	i;
	size_t	len;
	char	*str;

	len = 0;
	while (!is_delims(src[len], delims) && src[len])
		len++;
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		str[i] = src[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

static char	**free_split(char **split)
{
	size_t	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
	return (NULL);
}

char	**split_by_delims(char const *str, const char *delims)
{
	size_t	i;
	size_t	j;
	size_t	row;
	char	**split;

	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	split = row_malloc_split(str, delims, &row);
	if (split == NULL)
		return (NULL);
	while (i < row)
	{
		while (is_delims(str[j], delims))
			j++;
		split[i] = ft_strdup_split(&str[j], delims);
		if (split[i] == NULL)
			return (free_split(split));
		i++;
		while (!is_delims(str[j], delims) && str[j])
			j++;
	}
	return (split);
}
