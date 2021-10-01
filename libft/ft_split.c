#include "libft.h"

static char	**row_malloc_split(char const *str, char c, int *row)
{
	int		len;
	int		i;
	char	**split;

	i = 0;
	len = 0;
	while (str[i])
	{
		if (str[i] != c)
		{
			while (str[i] != c && str[i])
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
	split[len] = 0;
	*row = len;
	return (split);
}

static char	*ft_strdup_split(char const *src, char c)
{
	int		i;
	int		len;
	char	*str;

	len = 0;
	while (src[len] != c && src[len])
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
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		j;
	int		row;
	char	**split;

	if (!s)
		return (NULL);
	i = 0;
	j = 0;
	split = row_malloc_split(s, c, &row);
	if (split == NULL)
		return (NULL);
	while (i < row)
	{
		while (s[j] == c)
			j++;
		split[i] = ft_strdup_split(&s[j], c);
		if (split[i] == NULL)
			return (free_split(split));
		i++;
		while (s[j] != c && s[j])
			j++;
	}
	return (split);
}
