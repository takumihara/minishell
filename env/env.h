#ifndef ENV_H
#define ENV_H

# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <stdbool.h>

typedef struct s_env_var {
	char				*key;
	char				*value;
	struct s_env_var	*next;
}	t_env_var;

#endif
