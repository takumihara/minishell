#include "../utils/get_next_line.h"

int	x_get_next_line(int fd, char **line)
{
	t_gnl_status	status;

	status = get_next_line(fd, line);
	if (status == GNL_STATUS_ERROR_MALLOC)
		perror_exit("malloc", EXIT_FAILURE);
	if (status == GNL_STATUS_ERROR_READ)
		perror_exit("read", EXIT_FAILURE);
	return (status);
}
