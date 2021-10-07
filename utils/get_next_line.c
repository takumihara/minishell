#include "get_next_line_utils.h"
#include <stdio.h>
#include <string.h>

// todo: aggregate with one in utils
static char	*str_append(char **dst, const char *src, size_t l)
{
	char	*tmp;
	size_t	i;
	size_t	j;

	tmp = (char *)malloc(sizeof(char) * (ft_strlen(*dst) + l + 1));
	if (!tmp)
	{
		free(*dst);
		*dst = NULL;
		return (NULL);
	}
	i = 0;
	while ((*dst)[i])
		*tmp++ = (*dst)[i++];
	j = 0;
	while (j < l)
		*tmp++ = src[j++];
	*tmp = 0;
	free(*dst);
	*dst = tmp - (i + j);
	return (*dst);
}

static t_fd	*new_fd(t_fd **data, int fd)
{
	t_fd	*new;
	t_fd	*tmp;

	tmp = *data;
	new = malloc(sizeof(t_fd));
	if (!new)
		return (NULL);
	new->fd = fd;
	new->next = NULL;
	new->prev = NULL;
	new->buff[0] = '\0';
	while (tmp && tmp->next)
		tmp = tmp->next;
	if (!tmp)
	{
		*data = new;
		return (new);
	}
	else
	{
		new->prev = tmp;
		tmp->next = new;
		return (new);
	}
}

static int	check_fd(t_fd **data, int fd, t_fd **tmp, char **line)
{
	t_fd	*itr;

	itr = *data;
	while (itr)
	{
		if (itr->fd == fd)
		{
			*tmp = itr;
			return (1);
		}
		itr = itr->next;
	}
	*tmp = new_fd(data, fd);
	if (!*tmp)
	{
		*line = NULL;
		return (0);
	}
	return (1);
}

static int	check_data(t_fd *tmp, char **line)
{
	size_t	i;

	if (tmp->buff[0])
	{
		if (ft_strchr_gnl(tmp->buff, '\n', &i))
		{
			if (!str_append(line, tmp->buff, i + 1))
				return (1);
			ft_memmove(tmp->buff, &tmp->buff[i + 1],
					   ft_strlen(&tmp->buff[i + 1]) + 1);
			return (1);
		}
		if (!str_append(line, tmp->buff, ft_strlen(tmp->buff)))
			return (1);
	}
	return (0);
}

char	*get_next_line(int fd)
{
	static t_fd	*data;
	t_fd		*tmp;
	size_t		i;
	char		*line;
	ssize_t		rtn;

	if (!assign_mem((void **)&line, malloc(sizeof(char) * 1)))
		return (NULL);
	line[0] = '\0';
	if (!check_fd(&data, fd, &tmp, &line) || check_data(tmp, &line))
		return (line);
	while (1)
	{
		rtn = read(fd, tmp->buff, BUFFER_SIZE);
		tmp->buff[rtn] = '\0';
		if (rtn <= 0)
			return (rm_node(&data, &tmp, line, rtn));
		if (ft_strchr_gnl(tmp->buff, '\n', &i))
			break ;
		if (!str_append(&line, tmp->buff, ft_strlen(tmp->buff)))
			return (NULL);
	}
	str_append(&line, tmp->buff, i + 1);
	ft_memmove(tmp->buff, &tmp->buff[i + 1], ft_strlen(&tmp->buff[i + 1]) + 1);
	return (line);
}
