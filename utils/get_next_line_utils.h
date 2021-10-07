#ifndef GET_NEXT_LINE_UTILS_H
# define GET_NEXT_LINE_UTILS_H

# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>
# include <stdlib.h>

# include "../libft/libft.h"
# include "../utils/utils.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 64
# endif

typedef struct s_fd {
	int			fd;
	char		buff[(unsigned long long)BUFFER_SIZE + 1];
	struct s_fd	*next;
	struct s_fd	*prev;
}	t_fd;

int		ft_strchr_gnl(const char *str, int c, size_t *i);
char	*rm_node(t_fd **data, t_fd **tmp, char *line, ssize_t rtn);

#endif //GET_NEXT_LINE_UTILS_H
