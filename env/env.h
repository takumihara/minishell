#ifndef ENV_H
#define ENV_H

typedef struct s_env_var {
	char				*key;
	char				*value;
	struct s_env_var	*next;
}	t_env_var;

#endif
