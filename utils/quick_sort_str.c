#include "utils.h"

static void	swap(char **a, char **b)
{
	char	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	quick_sort_str(char **array, size_t left, size_t right)
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
		quick_sort_str(array, left, i - 1);
	if (j != right)
		quick_sort_str(array, j + 1, right);
}
