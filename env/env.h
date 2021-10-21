#ifndef ENV_H
#define ENV_H

# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <stdbool.h>
# include "../libft/libft.h"
# include "../utils/utils.h"
# include "../wrapper/x.h"

# define MALLOC_ERROR -1

typedef struct s_env_var {
	char				*key;
	char				*value;
	struct s_env_var	*next;
}	t_env_var;

// env_list.c
t_env_var	*init_env_var(char *key, char *value);
int			delete_env_lst(t_env_var *env_vars, char *key, char *value);
t_env_var	*init_env_lst(void);

// env_utils.c
char		*get_env_value(char *key, t_env_var *env_var);
void		register_env_var(char *key, char *value, t_env_var **env_vars);

// register_env_var_from_literal.c
void		register_env_var_from_literal(const char *str_key, const char *str_value, int num_value, t_env_var **env_vars);

#endif
