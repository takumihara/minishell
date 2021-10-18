#ifndef GET_CMD_PATH_H
#define GET_CMD_PATH_H

#include <stdbool.h>

#include "execute.h"

typedef struct s_sep_list	t_sep_list;

struct s_sep_list {
	int			sep_index;
	t_sep_list	*next;
};

#endif //GET_CMD_PATH_H
