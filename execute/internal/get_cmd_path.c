#include "../execute.h"
#include "execute_internal.h"
#include "split_path_from_env.h"

static char	*get_proper_mode_path(char **paths, char *command);

char	*get_cmd_path(t_executor *e, char *command)
{
	const char	*path_from_env = get_env_value("PATH", *e->env_vars);
	char		**paths;
	char		*path;

	if (ft_strchr(command, '/') || !path_from_env)
		return (x_strdup(command));
	paths = split_path_from_env(path_from_env);
	if (!paths)
		return (x_strdup(command));
	path = get_proper_mode_path(paths, command);
	return (path);
}

char	*get_proper_mode_path(char **paths, char *command)
{
	char	*not_executable_path;
	char	*path;
	int		i;

	i = -1;
	not_executable_path = NULL;
	while (1)
	{
		if (!paths[++i])
		{
			free_2d_array((void ***) &paths);
			return (not_executable_path);
		}
		path = strjoin_three(paths[i], "/", command);
		if (access(path, X_OK) == 0 && !is_dir(path))
			break ;
		if (!not_executable_path && access(path, F_OK) == 0 && !is_dir(path))
			not_executable_path = path;
		else
			free(path);
	}
	free_2d_array((void ***) &paths);
	return (path);
}
