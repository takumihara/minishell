#include "expander.h"

static bool	is_set(char c, char const *set)
{
	size_t	j;

	j = 0;
	while (set[j])
	{
		if (set[j] == c)
			return (true);
		j++;
	}
	return (false);
}

static char	**row_malloc_split(char const *str, const char *set, size_t *row)
{
	size_t	len;
	size_t	i;
	char	**split;

	i = 0;
	len = 0;
	while (str[i])
	{
		if (!is_set(str[i], set))
		{
			while (!is_set(str[i], set) && str[i])
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

static char	*ft_strdup_split(char const *src, const char *set)
{
	size_t	i;
	size_t	len;
	char	*str;

	len = 0;
	while (!is_set(src[len], set) && src[len])
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

char	**word_split(char const *str, const char *set)
{
	size_t	i;
	size_t	j;
	size_t	row;
	char	**split;

	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	split = row_malloc_split(str, set, &row);
	if (split == NULL)
		return (NULL);
	while (i < row)
	{
		while (is_set(str[j], set))
			j++;
		split[i] = ft_strdup_split(&str[j], set);
		if (split[i] == NULL)
			return (free_split(split));
		i++;
		while (!is_set(str[j], set) && str[j])
			j++;
	}
	return (split);
}
