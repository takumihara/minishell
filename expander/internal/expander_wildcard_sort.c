#include "expander_internal.h"

static void	swap(t_wildcard *a, t_wildcard *b)
{
	char	*tmp;

	tmp = a->data;
	a->data = b->data;
	b->data = tmp;
}

static void	quick_sort_pattern_list(t_wildcard *left, t_wildcard *right)
{
	const char	*pivot = left->data;
	t_wildcard	*i;
	t_wildcard	*j;

	i = left;
	j = right;
	if (left == right || left->prev == right)
		return ;
	while (1)
	{
		while (ft_strcmp(i->data, pivot) < 0)
			i = i->next;
		while (ft_strcmp(j->data, pivot) > 0)
			j = j->prev;
		if (i == j || i->prev == j)
			break ;
		swap(i, j);
		i = i->next;
		j = j->prev;
	}
	if (i)
		quick_sort_pattern_list(left, i->prev);
	if (j != right)
		quick_sort_pattern_list(j->next, right);
}

void	sort_pattern(t_wildcard *pattern_list)
{
	t_wildcard	*last;

	last = pattern_lstlast(pattern_list);
	quick_sort_pattern_list(pattern_list, last);
}
