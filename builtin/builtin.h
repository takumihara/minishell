#ifndef BUILTIN_H
# define BUILTIN_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>

# define BUILTIN_MALLOC_ERROR -1

# define EXPORT_ARG_ERROR 0
# define UNSET_ARG_ERROR 1

# include "../libft/libft.h"
# include "../utils/utils.h"
# include "../execute/execute.h"
# include "../expander/expander.h"
# include "../env/env.h"
# include "../wrapper/x.h"

# define EXIT_STATUS_OUT_OF_RANGE 255

int		builtin_cd(int argc, char **argv, int no_use, t_env_var **env_vars);
int		builtin_pwd(int argc, char **argv, int no_use, t_env_var **env_vars);
int		builtin_exit(int argc, char **argv, int last_exit_status, t_env_var **env_vars);
int		builtin_echo(int argc, char **argv, int no_use, t_env_var **env_vars);
int		builtin_export(int argc, char **argv, t_env_var **env_vars);
int		builtin_env(t_env_var *env_vars);
int		builtin_unset(int argc, char **argv, t_env_var **env_vars);

bool	is_valid_argument(char *argv, size_t len, int error_type);

#endif //BUILTIN_H
