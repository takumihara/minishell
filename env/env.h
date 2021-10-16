#ifndef ENV_H
#define ENV_H

# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <stdbool.h>
# include "../libft/libft.h"
# include "../utils/utils.h"

typedef struct s_env_var {
	char				*key;
	char				*value;
	struct s_env_var	*next;
}	t_env_var;

t_env_var	*init_env_var(char *key, char *value);
int			delete_env_lst(t_env_var *env_vars, char *key, char *value);
t_env_var	*init_env_lst(void);

#endif
