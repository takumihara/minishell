#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>

# include "utils.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 64
# endif

typedef enum e_gnl_status {
	GNL_STATUS_ERROR_READ = -2,
	GNL_STATUS_ERROR_MALLOC = -1,
	GNL_STATUS_DONE = 0,
	GNL_STATUS_SUCCESS = 1,
	GNL_STATUS_NOT_FINISHED = 2,
}	t_gnl_status;

int		get_next_line(int fd, char **line);

#endif
