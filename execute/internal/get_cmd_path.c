#include "../execute.h"
#include "execute_internal.h"
#include "split_path_from_env.h"

static char	*get_proper_mode_path(char **paths, char *command);

bool	get_cmd_path(t_executor *e, char *command, char **path)
{
	const char	*path_from_env = get_env_value("PATH", *e->env_vars);
	char		**paths;

	if (ft_strchr(command, '/') || !path_from_env)
	{
		*path = x_strdup(command);
		return (true);
	}
	paths = split_path_from_env(path_from_env);
	if (!paths)
	{
		*path = x_strdup(command);
		return (true);
	}
	*path = get_proper_mode_path(paths, command);
	if (*path == NULL)
		return (false);
	return (true);
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
