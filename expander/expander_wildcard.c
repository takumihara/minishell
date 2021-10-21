#include "expander.h"

char	*append_wildcard_strings(char *dst, char *src, const char *data, t_expander *e)
{
	if (dst == data)
		return (x_strdup(src));
	else
	{
		dst = strappend(dst, " ", 1);
		dst = strappend(dst, src, ft_strlen(src));
		return (dst);
	}
}

bool	is_match_pattern(const char *data, size_t len, char *name)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (i < len)
	{
		if (data[i] == '\"' || data[i] == '\'')
		{	
			i++;
			continue ;
		}
		if (data[i++] != name[j++])
			return (false);
	}
	return (true);
}

static void	swap(char **a, char **b)
{
	char	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	quick_sort(char **array, size_t left, size_t right)
{
	const char	*pivot = array[left];
	size_t		i;
	size_t		j;

	i = left;
	j = right;
	if (left >= right)
		return ;
	while (1)
	{
        while (ft_strcmp(array[i], pivot) < 0) 
            i++;
        while (ft_strcmp(array[j], pivot) > 0)
            j--;
        if (i >= j)
            break ;
        swap(&array[i], &array[j]);
        i++;
        j--;
    }
	if (i)
		quick_sort(array, left, i - 1);
	if (j != right)
		quick_sort(array, j + 1, right);
}

char	*sort_strings(char *src, t_expander *e, char *data)
{
	char	**wildcard_array;
	size_t	word_num;
	char	*rtn;
	size_t	i;

	free(data);
	wildcard_array = x_split(src, ' ');
	free(src);
	word_num = 0;
	while (wildcard_array[word_num])
		word_num++;
	quick_sort(wildcard_array, 0, word_num - 1);
	rtn = NULL;
	i = 0;
	while (i < word_num)
		rtn = append_wildcard_strings(rtn, wildcard_array[i++], NULL, e);
	free_2d_array((void ***)&wildcard_array);
	return (rtn);
}
