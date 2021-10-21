#ifndef SPLIT_PATH_FROM_ENV_H
# define SPLIT_PATH_FROM_ENV_H

# include <stdbool.h>

typedef struct s_sep_list	t_sep_list;

struct s_sep_list {
	int			sep_index;
	t_sep_list	*next;
};

char	**split_path_from_env(const char *path_from_env);

#endif //SPLIT_PATH_FROM_ENV_H
