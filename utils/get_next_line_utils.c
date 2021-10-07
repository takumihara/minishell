#include "get_next_line_utils.h"
#include <string.h>

int	ft_strchr_gnl(const char *str, int c, size_t *i)
{
	size_t	index;

	index = 0;
	while (str[index] || c == '\0')
	{
		if (str[index] == c)
		{
			*i = index;
			return (1);
		}
		index++;
	}
	return (0);
}

char	*rm_node(t_fd **data, t_fd **tmp, char *line, ssize_t rtn)
{
	t_fd	*tmp_prev;

	if (*line)
		return (line);
	tmp_prev = (*tmp)->prev;
	if (tmp_prev)
		tmp_prev->next = (*tmp)->next;
	else
		*data = (*tmp)->next;
	if ((*tmp)->next)
		(*tmp)->next->prev = tmp_prev;
	free(*tmp);
	*tmp = NULL;
	if (rtn == -1 || *line == '\0')
	{
		free(line);
		return (NULL);
	}
	return (line);
}
