#include "expander_internal.h"

void	delete_pattern_list(t_wildcard *pattern_list)
{
	t_wildcard	*tmp_next;

	while (pattern_list)
	{
		tmp_next = pattern_list->next;
		free(pattern_list->data);
		free(pattern_list);
		pattern_list = tmp_next;
	}
}

static t_wildcard	*init_pattern_list(const char *data, t_wildcard *prev)
{
	t_wildcard	*new;

	new = malloc(sizeof(*new));
	new->data = strdup(data);
	new->prev = prev;
	new->next = NULL;
	if (prev)
		prev->next = new;
	return (new);
}

static t_wildcard	*wildcard_lstlast(t_wildcard *pattern_list)
{
	while (pattern_list)
	{
		if (!pattern_list->next)
			break ;
		pattern_list = pattern_list->next;
	}
	return (pattern_list);
}

void	append_pattern_list(t_wildcard **pattern_list, const char *data)
{
	t_wildcard	*target;

	target = wildcard_lstlast(*pattern_list);
	if (target)
		target->next = init_pattern_list(data, target);
	else
		*pattern_list = init_pattern_list(data, NULL);
}
