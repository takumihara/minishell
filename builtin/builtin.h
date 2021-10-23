#ifndef BUILTIN_H
# define BUILTIN_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>

# define EXPORT_ARG_ERROR 0
# define UNSET_ARG_ERROR 1

# include "../libft/libft.h"
# include "../utils/utils.h"
# include "../execute/execute.h"
# include "../expander/expander.h"
# include "../env/env.h"
# include "../wrapper/x.h"
// todo: maybe I wanna move the place of builtin dir
# include "../execute/internal/execute_internal.h"

int		builtin_cd(int argc, char **argv, t_env_var **env_vars);
int		builtin_pwd(void);
int		builtin_exit(t_executor *e, int argc, char **argv);
int		builtin_echo(int argc, char **argv);
int		builtin_export(int argc, char **argv, t_env_var **env_vars);
int		builtin_env(t_env_var *env_vars);
int		builtin_unset(int argc, char **argv, t_env_var **env_vars);

bool	is_valid_argument(char *argv, size_t len, int error_type);

#endif //BUILTIN_H
