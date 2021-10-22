#include "../execute.h"
#include "execute_internal.h"
#include "split_path_from_env.h"

char	*get_cmd_path(t_executor *e, char *command)
{
	const char	*path_from_env = get_env_value("PATH", *e->env_vars);
	char		**paths;
	char		*path;
	char		*tmp_path;
	int			i;

	if (ft_strchr(command, '/') || !path_from_env)
		return (x_strdup(command));
	paths = split_path_from_env(path_from_env);
	if (!paths)
		return (x_strdup(command));
	i = -1;
	tmp_path = NULL;
	while (1)
	{
		if (!paths[++i])
		{
			free_2d_array((void ***) &paths);
			return (tmp_path);
		}
		path = strjoin_three(paths[i], "/", command);
		if (access(path, X_OK) == 0 && !is_dir(path))
			break;
		if (!tmp_path && access(path, F_OK) == 0 && !is_dir(path))
			tmp_path = path;
		else
			free(path);
	}
	free_2d_array((void ***) &paths);
	return (path);
}
