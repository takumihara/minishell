#include "get_cmd_path.h"
#include "../wrapper/x.h"

static void	destroy_sep_list(t_sep_list *head)
{
	t_sep_list	*next;

	while (head)
	{
		next = head->next;
		free(head);
		head = next;
	}
}

static t_sep_list *new_sep_list(int sep_index)
{
	t_sep_list	*new;

	new = x_malloc(sizeof(*new));
	new->next = NULL;
	new->sep_index = sep_index;
	return (new);
}

static void create_paths(char **paths, const char *path_from_env, t_sep_list *sep_list)
{
	size_t	start;
	int		i;
	size_t	len;

	start = sep_list->sep_index;
	sep_list = sep_list->next;
	i = -1;
	while (sep_list)
	{
		len = sep_list->sep_index - start - 1;
		if (len == 0)
			paths[++i] = x_strdup(".");
		else
			paths[++i] = x_substr(path_from_env, start + 1, len);
		start = sep_list->sep_index;
		sep_list = sep_list->next;
	}
	paths[++i] = NULL;
}

static t_sep_list *create_sep_list(const char *path_from_env, int *list_len)
{
	t_sep_list	*head;
	t_sep_list	*tail;
	int			i;

	head = new_sep_list(-1);
	tail = head;
	i = -1;
	*list_len = 1;
	while (path_from_env[++i])
	{
		if (path_from_env[i] == ':')
		{
			tail->next = new_sep_list(i);
			tail = tail->next;
			*list_len += 1;
		}
	}
	tail->next = new_sep_list(i);
	return (head);
}

char **split_path_from_env(const char *path_from_env)
{
	char		**paths;
	t_sep_list	*sep_list;
	int			list_len;

	sep_list = create_sep_list(path_from_env, &list_len);
	paths = x_malloc(sizeof(*paths) * (list_len + 1));
	create_paths(paths, path_from_env, sep_list);
	destroy_sep_list(sep_list);
	return (paths);
}

char	*get_cmd_path(t_executor *e, char *command)
{
	char 	*path_from_env;
	char	**paths;
	char	*path;
	int		i;

	path_from_env = get_env_value("PATH", *e->env_vars);
	if (!path_from_env)
		return (x_strdup(command));
	paths = split_path_from_env(path_from_env);
	if (!paths[0])
		return (x_strdup(command));
	i = -1;
	while (1)
	{
		if (!paths[++i])
		{
			free_2d_array((void ***) &paths);
			return (NULL);
		}
		path = strjoin_three(paths[i], "/", command);
		if (access(path, F_OK) == 0)
			break ;
		free(path);
	}
	free_2d_array((void ***) &paths);
	return (path);
}