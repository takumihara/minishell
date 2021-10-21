#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include "utils.h"
# include "../libft/libft.h"

#define BUFFER_SIZE 4096

char	*read_all(int fd)
{
	char	buff[BUFFER_SIZE + 1];
	ssize_t	read_bytes;
	char	*content;

	content = ft_strdup("");
	if (!content)
		perror_exit("malloc", EXIT_FAILURE);
	while (1)
	{
		read_bytes = read(fd, buff, BUFFER_SIZE);
		buff[read_bytes] = '\0';
		if (read_bytes == -1)
			perror_exit("read", EXIT_FAILURE);
		if (read_bytes == 0)
			break ;
		content = strappend(content, buff, read_bytes);
		if (!content)
			perror_exit("malloc", EXIT_FAILURE);
	}
	return (content);
}
